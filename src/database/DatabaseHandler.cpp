/*
 * Copyright (C) 2013 Emeric Poupon
 *
 * This file is part of LMS.
 *
 * LMS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LMS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LMS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Wt/Auth/Dbo/AuthInfo>
#include <Wt/Auth/Dbo/UserDatabase>
#include <Wt/Auth/AuthService>
#include <Wt/Auth/HashFunction>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/PasswordStrengthValidator>
#include <Wt/Auth/PasswordVerifier>

#include "logger/Logger.hpp"

// Db types
#include "AudioTypes.hpp"
#include "VideoTypes.hpp"
#include "MediaDirectory.hpp"
#include "User.hpp"

#include "DatabaseHandler.hpp"

namespace Database {


namespace {
	Wt::Auth::AuthService authService;
	Wt::Auth::PasswordService passwordService(authService);
}


void
Handler::configureAuth(void)
{
	authService.setEmailVerificationEnabled(true);

	Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
	verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(8));
	passwordService.setVerifier(verifier);
	passwordService.setAttemptThrottlingEnabled(true);

	Wt::Auth::PasswordStrengthValidator* strengthValidator = new Wt::Auth::PasswordStrengthValidator();
	// Reduce some constraints...
	strengthValidator->setMinimumLength( Wt::Auth::PasswordStrengthValidator::PassPhrase, 4);
	strengthValidator->setMinimumLength( Wt::Auth::PasswordStrengthValidator::OneCharClass, 4);
	strengthValidator->setMinimumLength( Wt::Auth::PasswordStrengthValidator::TwoCharClass, 4);
	strengthValidator->setMinimumLength( Wt::Auth::PasswordStrengthValidator::ThreeCharClass, 4 );
	strengthValidator->setMinimumLength( Wt::Auth::PasswordStrengthValidator::FourCharClass, 4  );

	passwordService.setStrengthValidator(strengthValidator);
}

const Wt::Auth::AuthService&
Handler::getAuthService()
{
	return authService;
}

const Wt::Auth::PasswordService&
Handler::getPasswordService()
{
	return passwordService;
}


Handler::Handler(boost::filesystem::path db)
:
_dbBackend( db.string() )
{
	_session.setConnection(_dbBackend);
	_session.mapClass<Database::Genre>("genre");
	_session.mapClass<Database::Track>("track");
	_session.mapClass<Database::Video>("video");
	_session.mapClass<Database::MediaDirectory>("media_directory");
	_session.mapClass<Database::MediaDirectorySettings>("media_directory_settings");

	_session.mapClass<Database::User>("user");
	_session.mapClass<Database::AuthInfo>("auth_info");
	_session.mapClass<Database::AuthInfo::AuthIdentityType>("auth_identity");
	_session.mapClass<Database::AuthInfo::AuthTokenType>("auth_token");

	try {
	        _session.createTables();
		_dbBackend.executeSql("CREATE INDEX artist_name_idx ON track(artist_name)");
		_dbBackend.executeSql("CREATE INDEX release_name_idx ON track(release_name)");
		_dbBackend.executeSql("CREATE INDEX genre_name_idx ON genre(name)");
	}
	catch(std::exception& e) {
		LMS_LOG(MOD_DB, SEV_ERROR) << "Cannot create tables: " << e.what();
	}

	_dbBackend.executeSql("pragma journal_mode=WAL");

	_users = new UserDatabase(_session);
}

Handler::~Handler()
{
	delete _users;
}

Wt::Auth::AbstractUserDatabase&
Handler::getUserDatabase()
{
	return *_users;
}

User::pointer
Handler::getCurrentUser()
{
	if (_login.loggedIn())
		return getUser(_login.user());
	else
		return User::pointer();
}

User::pointer
Handler::getUser(const Wt::Auth::User& authUser)
{

	if (!authUser.isValid()) {
		LMS_LOG(MOD_DB, SEV_ERROR) << "Handler::getUser: invalid authUser";
		return User::pointer();
	}

	Wt::Dbo::ptr<AuthInfo> authInfo = _users->find(authUser);

	User::pointer user = authInfo->user();

	if (!user) {
		user = _session.add(new User());
		authInfo.modify()->setUser(user);
	}

	return user;
}


} // namespace Database

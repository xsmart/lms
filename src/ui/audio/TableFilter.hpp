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

#ifndef TABLE_FILTER_HPP
#define TABLE_FILTER_HPP


#include <Wt/Dbo/QueryModel>
#include <Wt/WTableView>

#include "Filter.hpp"
#include "database/DatabaseHandler.hpp"

namespace UserInterface {

class TableFilter : public Wt::WTableView, public Filter
{

	public:
		TableFilter(Database::Handler& db, Database::SearchFilter::Field field, std::vector<Wt::WString> displayName, Wt::WContainerWidget* parent = 0);

		// Set constraints on this filter
		void refresh(Database::SearchFilter& filter);

		// Get constraints created by this filter
		void getConstraint(Database::SearchFilter& filter);

		void layoutSizeChanged (int width, int height);

		typedef Wt::Signal<void> SigDoubleClicked;

		SigDoubleClicked&	sigDoubleClicked() { return _sigDoubleClicked; }

	protected:

		SigDoubleClicked			_sigDoubleClicked;

		Database::Handler&			_db;
		Database::SearchFilter::Field		_field;

		// Name, track count
		typedef boost::tuple<std::string, int>  ResultType;
		Wt::Dbo::QueryModel< ResultType >	_queryModel;

};

} // namespace UserInterface

#endif


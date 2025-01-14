/*
 * Copyright (C) 2015 Emeric Poupon
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

#ifndef UI_AUDIO_MOBILE_HPP
#define UI_AUDIO_MOBILE_HPP

#include <Wt/WContainerWidget>

#include "audio/Audio.hpp"

#include "ArtistSearch.hpp"
#include "ReleaseSearch.hpp"
#include "TrackSearch.hpp"
#include "TrackReleaseView.hpp"

namespace UserInterface {
namespace Mobile {

class Audio : public UserInterface::Audio
{
	public:
		Audio(Wt::WContainerWidget *parent = 0);

		void search(std::string text);

	private:

		ArtistSearch*	_artistSearch;
		ReleaseSearch*	_releaseSearch;
		TrackSearch*	_trackSearch;
		TrackReleaseView*	_trackReleaseView;
};

} // namespace Mobile
} // namespace UserInterface

#endif

/* Copyright (C) 2015  Charlie Fyvie-Gauld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __RACKDESC_H_1439063050__
#define __RACKDESC_H_1439063050__

#include "framework/fwcommon.hpp"
namespace strangeio {
namespace config {

typedef struct {

	typedef struct {

		typedef struct {
			int num_workers;
		} s_threading;
		s_threading threads;

	} s_system;

	typedef struct {

		typedef struct {
			std::string label;
			std::string port;
		} s_controller;

		std::vector<s_controller> controllers;

	} s_midi;

	typedef struct {

		typedef struct {
			std::string from, plug, to, jack;
		} s_link;

		typedef struct {

			typedef struct {
				std::string type, value;
			} s_config;

			typedef struct {
				std::string name, module;
				int code;
			} s_binding;

			std::string label, unit, library;
			std::vector<s_config> configs;
			std::vector<s_binding> bindings;
		} s_unit;

		std::vector<std::string> mainlines;
		std::vector<s_link> daisychains;
		std::vector<s_unit> units;

	} s_setup;

	s_system system;
	s_midi midi;
	s_setup setup;


} description;



} // Config
} // StrangeIO
#endif
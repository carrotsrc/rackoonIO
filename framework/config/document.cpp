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
#include <fstream>
#include "framework/config/document.hpp"

using namespace strangeio::config;

using Pval = picojson::value;
using Pobj = picojson::object;
using Pnull = picojson::null;
using Parr = picojson::array;


std::unique_ptr<description> document::load(std::string path) {
	Pval v;
	std::string config = load_file(path);
	auto err = picojson::parse(v, config);
	if(!err.empty())
		std::cout << err << std::endl;


	m_rack = std::unique_ptr<description>(new description);
	parse_document(v, root);

	return std::move(m_rack);
}

std::string document::load_file(std::string path) {

	std::ifstream f(path);

	if(f.fail()) {
		std::cerr << "Config file `" << path <<"` not found!" << std::endl;
		exit(1);
	}

	std::string config(
			(std::istreambuf_iterator<char>(f)), 
			std::istreambuf_iterator<char>()
	);

	f.close();
	return config;
}

void document::parse_document(const Pval& v, document::element_type element) {

	Pval cv;

	if(v.is<picojson::object>()) {
		auto& obj = v.get<Pobj>();
			for (auto& i : obj) {
				if(i.first == "system" && element == root) {

					parse_document(i.second, system);

				} else if(i.first == "rack" && element == root) {

					parse_rack(i.second);

				} else if(i.first == "midi" && element == root) {

					for (auto& mit : i.second.get<Pobj>()) {
						m_rack->midi.controllers.push_back({
							.label = mit.first,
							.port = mit.second.get<std::string>(),
						});
					}

				} else if(i.first == "threads" && element == system) {

					cv = i.second.get("workers");
					if(!cv.is<Pnull>()) {
						m_rack->system.threads.num_workers = static_cast<int>(cv.get<double>());
					}

					return;
				}
		}
	}
}

void document::parse_rack(const Pval& v) {
	std::string fm, pl, to, jk;

	Pval cv;

	// get the virtual mainlines plugs
	auto mlines = v.get("mainlines").get<Parr>();

	for(const auto & mit : v.get("mainlines").get<Parr>()) {
		m_rack->setup.mainlines.push_back(mit.get("plug").get<std::string>());
	}

	// get the daisy chains
	for(const auto&  it : v.get("daisychains").get<Parr>()) {

		fm = it.get("from").get<std::string>();
		pl = it.get("plug").get<std::string>();
		to = it.get("to").get<std::string>();
		jk = it.get("jack").get<std::string>();

		m_rack->setup.daisychains.push_back({
			.from = fm, .plug = pl, .to = to, .jack = jk
		});

		auto uv = v.get(to);
		if(uv.is<Pnull>())
			continue;

		parse_unit(to, uv);
	}
}

void document::parse_unit(std::string label, const Pval& v) {
	for(const auto& u : m_rack->setup.units) {
		if(u.label == label) {
			return;
		}
	}

	auto cv = v.get("library");
	std::string target;
	description::s_setup::s_unit unit;

	unit.label = label;


	if (!cv.is<Pnull>()) {
		unit.library = cv.get<std::string>();
	}

	cv = v.get("unit");
	if (!cv.is<Pnull>()) {
		unit.unit = cv.get<std::string>();
	}

	cv = v.get("bindings");
	if (!cv.is<Pnull>()) {
		parse_bindings(unit, cv);
	}

	cv = v.get("config");
	if(!cv.is<Pnull>()) {
		for(const auto & it : cv.get<Pobj>()) {
			unit.configs.push_back( {
				.type = it.first, 
				.value = it.second.get<std::string>()
			});
		}
	}
	m_rack->setup.units.push_back(unit);

}

void document::parse_bindings(description::s_setup::s_unit& unit, const Pval & v) {

	for (const auto & bit : v.get<Pobj>()) {
		if(bit.second.get("module").is<Pnull>() || bit.second.get("code").is<Pnull> ())
			continue;

		auto method = bit.first;
		auto module = bit.second.get("module").get<std::string>();
		auto code = static_cast<int>(bit.second.get("code").get<double>());

		unit.bindings.push_back({
			.name = method, .module = module, .code = code
		});
	}
}

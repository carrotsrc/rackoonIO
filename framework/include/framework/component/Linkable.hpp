#ifndef __LINKABLE_HPP_1440496152__
#define __LINKABLE_HPP_1440496152__
#include <vector>
#include "framework/component/Component.hpp"
#include "framework/fwcommon.hpp"
#include "framework/memory/CachePtr.hpp"

namespace StrangeIO {
namespace Component {

class Linkable; // Forward declaration

struct LinkIn {
	std::string label;
	unsigned int id;
	Linkable* unit;
	bool connected;
};

struct LinkOut {
	std::string label;
	unsigned int id;
	LinkIn* to;
	bool connected;
};

class Linkable {
public:
	Linkable();

	bool connect(int id, LinkIn*);
	void disconnect(int id);

	const int has_input(std::string label) const;
	const LinkIn* get_input(int id) const;

	const int has_output(std::string label) const;
	const LinkOut* get_output(int id) const;
	virtual void sync_line(Profile & profile, SyncFlag flags) = 0;
	virtual CycleState cycle_line(CycleType type) = 0;

protected:
	void add_output(std::string label);
	void add_input(std::string label);

	bool input_connected(unsigned int id) const;
	bool output_conncted(unsigned int id) const;

	bool feed_out(Memory::CachePtr samples, int id);
	virtual void feed_line(Memory::CachePtr samples, int line) = 0;

	const std::vector<LinkIn> & inputs() const;
	const std::vector<LinkOut> & outputs() const;

private:
	std::vector<LinkIn> m_inputs;
	unsigned int m_num_inputs;

	std::vector<LinkOut> m_outputs;
	unsigned int m_num_outputs;
};

} // Component
} // StrangeIO
#endif


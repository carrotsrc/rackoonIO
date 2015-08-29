#ifndef __UNIT_HPP_1440406068__
#define __UNIT_HPP_1440406068__

#include <memory>

#include "framework/component/Component.hpp"
#include "framework/component/Linkable.hpp"
#include "framework/component/RackUtilityInterface.hpp"

#include "framework/memory/CacheRw.hpp"

namespace StrangeIO {
namespace Component {

class Unit : public Linkable, public Memory::CacheRw {
public:
	Unit(UnitType utype, std::string umodel, std::string ulabel);
	

	void set_rack(RackUtilityInterface* rack);

	// Description and State methods
	UnitType utype() const;
	std::string umodel() const;
	std::string ulabel() const;
	ComponentState cstate() const;

	// Communication methods
	CycleState cycle_line(CycleType cycle);
	void sync_line(Profile & profile, SyncFlag flags = 0);
	void feed_line(Memory::CachePtr samples, int line) = 0;

	// Profiling
	const Profile& unit_profile() const;
	virtual void set_configuration(std::string key, std::string value) = 0;

protected:
	void change_cstate(ComponentState state);
	void register_metric(ProfileMetric type, int value);
	void register_metric(ProfileMetric type, float value);
	const Profile& line_profile() const;

	void log(std::string mout);
	virtual CycleState cycle() = 0;
	virtual CycleState init() = 0;

private:
	const UnitType m_utype;
	const std::string m_umodel, m_ulabel;
	ComponentState m_cstate;

	RackUtilityInterface* m_rack;

	Profile m_line_profile, m_unit_profile;

};

using unit_uptr = std::unique_ptr<Unit>;
using unit_sptr = std::shared_ptr<Unit>;
using unit_wptr = std::weak_ptr<Unit>;

} // Component
} // StrangeIO
#endif


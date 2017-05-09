#include "utilization/utilization.h"
#include <timer_session/connection.h>
#include <base/env.h>
#include <base/printf.h>
#include <base/process.h>
#include <util/xml_node.h>
#include <util/xml_generator.h>
#include <string>
#include <util/string.h>
#include <os/config.h>
namespace Utilization{
	Utilization::Utilization() {}

	double Utilization::utilization(int core) {
		switch(core){
			case 0: return util0;
			case 1: return util1;
			case 2: return util2;
			case 3: return util3;
			default: return -1;
		}
	}

	void Utilization::compute() {
		Mon_manager::Connection mon_manager;
		Timer::Connection timer;
		int timestamp=1000;
		while(true) {
			timer.msleep(timestamp-20);
			unsigned long long tmp0=mon_manager.get_idle_time(0).value;
			unsigned long long tmp1=mon_manager.get_idle_time(1).value;
			unsigned long long tmp2=mon_manager.get_idle_time(2).value;
			unsigned long long tmp3=mon_manager.get_idle_time(3).value;
			util0=100-(tmp0-prev0)/(10*timestamp);
			util1=100-(tmp1-prev1)/(10*timestamp);
			util2=100-(tmp2-prev2)/(10*timestamp);
			util3=100-(tmp3-prev3)/(10*timestamp);
			Genode::printf("util: %d %d %d %d\n",util0,util1,util2,util3);
			prev0=tmp0;
			prev1=tmp1;
			prev2=tmp2;
			prev3=tmp3;
		}
	}
}

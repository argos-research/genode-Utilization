#pragma once
#include <utilization/utilization_session.h>
#include "mon_manager/mon_manager_connection.h"
#include "mon_manager/mon_manager_client.h"
#include "mon_manager/mon_manager.h"
#include <base/signal.h>
#include <os/attached_ram_dataspace.h>
#include <os/server.h>
#include <root/component.h>
#include <timer_session/connection.h>
#include <util/string.h>
#include <cap_session/connection.h>
#include <trace_session/connection.h>
#include <base/thread_state.h>

namespace Utilization{
class Utilization
{
	public:
		Utilization();
		double utilization(int core);
		void compute();
	private:
		unsigned long long prev0;
		unsigned long long prev1;
		unsigned long long prev2;
		unsigned long long prev3;
		int util0;
		int util1;
		int util2;
		int util3;
		long long unsigned val;
		
};
}

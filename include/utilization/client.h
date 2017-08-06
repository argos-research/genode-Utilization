#pragma once

#include <utilization/utilization_session.h>
#include <base/rpc_client.h>
#include <base/printf.h>

namespace Utilization {

	struct Session_client : Genode::Rpc_client<Session>
	{
		Session_client(Genode::Capability<Session> cap)
		: Genode::Rpc_client<Session>(cap) { }

		int utilization(int core)
		{
			return call<Rpc_utilization>(core);
		}
	};
}

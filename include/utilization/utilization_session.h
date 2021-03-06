#pragma once

#include <session/session.h>
#include <base/rpc.h>

namespace Utilization {

	struct Session : Genode::Session
	{
		static const char *service_name() { return "utilization"; }

		virtual int utilization(int core) = 0;


		/*******************
		 ** RPC interface **
		 *******************/

		GENODE_RPC(Rpc_utilization, int, utilization, int);

		GENODE_RPC_INTERFACE(Rpc_utilization);
	};
}

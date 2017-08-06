#pragma once

#include <utilization/client.h>
#include <base/connection.h>

namespace Utilization {

	struct Connection : Genode::Connection<Session>, Session_client
	{
		Connection()
		:
			/* create session */
			Genode::Connection<Utilization::Session>(session("utilization, ram_quota=4K")),

			/* initialize RPC interface */
			Session_client(cap()) { }
	};
}

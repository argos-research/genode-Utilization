
#include <utilization/client.h>
#include <base/connection.h>

namespace Utilization {

	struct Connection : Genode::Connection<Session>, Session_client
	{
		Connection(Genode::Env &env)
		:
			/* create session */
			Genode::Connection<Utilization::Session>(env, session(env.parent(),
		                                                "ram_quota=6K, cap_quota=4")),

			/* initialize RPC interface */
			Session_client(cap()) { }
	};
}

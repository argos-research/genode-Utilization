#include <base/printf.h>
#include <base/env.h>
#include <base/sleep.h>
#include <cap_session/connection.h>
#include <root/component.h>
#include <base/rpc_server.h>
#include <utilization/utilization.h>
#include <utilization/utilization_session.h>

namespace Utilization {

	struct Session_component : Genode::Rpc_object<Session>
	{
		private:
			Utilization* _util=nullptr;
		public:
			double utilization(int core) {
				return _util->utilization(core);			
			}

			Session_component(Utilization *util)
			: Genode::Rpc_object<Session>()
			{
				_util = util;
			}
	};

	class Root_component : public Genode::Root_component<Session_component>
	{
		private:
			Utilization* _util=nullptr;
		protected:

			Session_component *_create_session(const char *args)
			{
				PDBG("creating hello session.");
				return new (md_alloc()) Session_component(_util);
			}

		public:

			Root_component(Genode::Rpc_entrypoint *ep,
			               Genode::Allocator *allocator,
					Utilization* util)
			: Genode::Root_component<Session_component>(ep, allocator)
			{
				PDBG("Creating root component.");
				_util=util;
			}
	};
}


using namespace Genode;

int main(void)
{
	Utilization::Utilization util;

	/*
	 * Get a session for the parent's capability service, so that we
	 * are able to create capabilities.
	 */
	Cap_connection cap;

	/*
	 * A sliced heap is used for allocating session objects - thereby we
	 * can release objects separately.
	 */
	static Sliced_heap sliced_heap(env()->ram_session(),
	                               env()->rm_session());

	/*
	 * Create objects for use by the framework.
	 *
	 * An 'Rpc_entrypoint' is created to announce our service's root
	 * capability to our parent, manage incoming session creation
	 * requests, and dispatch the session interface. The incoming RPC
	 * requests are dispatched via a dedicated thread. The 'STACK_SIZE'
	 * argument defines the size of the thread's stack. The additional
	 * string argument is the name of the entry point, used for
	 * debugging purposes only.
	 */
	enum { STACK_SIZE = 4096 };
	static Rpc_entrypoint ep(&cap, STACK_SIZE, "utilitzation_ep");

	static Utilization::Root_component utilization_root(&ep, &sliced_heap, &util);
	env()->parent()->announce(ep.manage(&utilization_root));

	util.compute();

	/* We are done with this and only act upon client requests now. */
	sleep_forever();

	return 0;
}

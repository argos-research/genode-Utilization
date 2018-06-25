#include <base/printf.h>
#include <base/env.h>
#include <base/sleep.h>
//#include <cap_session/connection.h>
#include <base/component.h>
#include <root/component.h>
#include <base/rpc_server.h>
#include <utilization/utilization.h>
#include <libc/component.h>
#include <utilization/utilization_session.h>

namespace Utilization {

	struct Session_component : Genode::Rpc_object<Session>
	{
		private:
			Utilization* _util=nullptr;
		public:
			enum { CAP_QUOTA = 2 };
			double utilization(int core) {
				return _util->utilization(core);			
			}

			Session_component(Utilization *util)
			: Genode::Rpc_object<Session>()
			{
				_util = util;
			}
			Session_component(const Session_component&);
			Session_component& operator = (const Session_component&);	
	};

	class Root_component : public Genode::Root_component<Session_component>
	{
		private:
			Utilization* _util=nullptr;
		protected:

			Session_component *_create_session(const char *)
			{
				Genode::log("creating hello session.");
				return new (md_alloc()) Session_component(_util);
			}

		public:

			Root_component(Genode::Entrypoint &ep,
			               Genode::Allocator &allocator,
					Utilization* util)
			: Genode::Root_component<Session_component>(ep, allocator)
			{
				Genode::log("Creating root component.");
				_util=util;
			}
			Root_component(const Root_component&);
			Root_component& operator = (const Root_component&);
	};
}


using namespace Genode;

struct Main
{	
	Libc::Env &_env;
	Genode::Entrypoint &_ep;
	Utilization::Utilization util {_env};

	/*
	 * Get a session for the parent's capability service, so that we
	 * are able to create capabilities.
	 */
	//Cap_connection cap;

	/*
	 * A sliced heap is used for allocating session objects - thereby we
	 * can release objects separately.
	 */
	Genode::Sliced_heap sliced_heap{_env.ram(),
	                               _env.rm()};

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
	//enum { STACK_SIZE = 4096 };
	//static Rpc_entrypoint ep(&cap, STACK_SIZE, "utilitzation_ep");

	Utilization::Root_component _utilization_root {_ep, sliced_heap, &util};
	
	Main(Libc::Env &env) : _env(env), _ep(_env.ep()){
		util.compute();
		_env.parent().announce(_ep.manage(_utilization_root));

	}


	/* We are done with this and only act upon client requests now. */
	

};

//void Component::construct(Genode::Env &env) { static Main main(env); }
void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] () { static Main main(env); });
}

#include <base/printf.h>
#include <base/env.h>
#include <base/sleep.h>
#include <base/component.h>
#include <root/component.h>
#include <base/rpc_server.h>
#include <utilization/utilization.h>
#include <libc/component.h>
#include <utilization/utilization_session.h>

namespace Utilization {
	struct Main;
	struct Session_component;
	struct Root_component;
}

struct Utilization::Session_component : Genode::Rpc_object<Session>
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

class Utilization::Root_component : public Genode::Root_component<Session_component>
{
	private:
		Utilization* _util { };
	protected:

		Session_component *_create_session(const char*)
		{
			return new (md_alloc()) Session_component(_util);
		}

	public:

		Root_component(Genode::Entrypoint &ep,
		               Genode::Allocator &alloc,
		               Utilization *util)
		:
			Genode::Root_component<Session_component>(ep, alloc)
		{
			_util=util;
		}
	Root_component(const Root_component&);
	Root_component& operator = (const Root_component&);	
};

struct Utilization::Main
{
	enum { ROOT_STACK_SIZE = 16*1024 };
	Genode::Env	&_env;
	Genode::Heap	heap	{ _env.ram(), _env.rm() };
	Utilization util { _env };
	Root_component utilization_root { _env.ep(), heap , &util};

	Main(Libc::Env &env_) : _env(env_)
	{
		_env.parent().announce(_env.ep().manage(utilization_root));
		util.compute();
	}
};

Genode::size_t Component::stack_size() { return 32*1024; }

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] () { static Utilization::Main main(env); });
}

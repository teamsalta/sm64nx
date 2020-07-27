#include <ultra64.h>
#include "sm64.h"
#include "graph_node.h"
#include "hook_geo.h"
#include "hook_dl.h"
#include "asset.h"
#include "xxhash64.h"
#include <unordered_map>

#define CMD_SIZE_SHIFT (sizeof(void*) >> 3)
#define CMD_PROCESS_OFFSET(offset) ((offset & 3) | ((offset & ~3) << CMD_SIZE_SHIFT))

#define cmd_u8(offset) (ip[CMD_PROCESS_OFFSET(offset)])

#define cmd_s16(offset) (*(s16*)&ip[CMD_PROCESS_OFFSET(offset)])

#define cmd_s32(offset) (*(s32*)&ip[CMD_PROCESS_OFFSET(offset)])

#define cmd_u32(offset) (*(u32*)&ip[CMD_PROCESS_OFFSET(offset)])

#define cmd_ptr(offset) (*(void**)&ip[CMD_PROCESS_OFFSET(offset)])

namespace sm64::hook::geo
{
	u64 fingerprint(const GeoLayout* layout, XXHash64* state)
	{
		Layout l((u8*)layout, state);
		return l.fingerprint();
	}

	u64 fingerprint(const GeoLayout* layout)
	{
		XXHash64 state(0);
		return fingerprint(layout, &state);
	}

	typedef void (Layout::*CommandProc)();

	static CommandProc jumpTable[] = {
	    &Layout::branch_and_link,
	    &Layout::end,
	    &Layout::branch,
	    &Layout::ret,
	    &Layout::open_node,
	    &Layout::close_node,
	    &Layout::assign_as_view,
	    &Layout::update_node_flags,
	    &Layout::node_root,
	    &Layout::node_ortho_projection,
	    &Layout::node_perspective,
	    &Layout::node_start,
	    &Layout::node_master_list,
	    &Layout::node_level_of_detail,
	    &Layout::node_switch_case,
	    &Layout::node_camera,
	    &Layout::node_translation_rotation, // variable
	    &Layout::node_translation,		// variable
	    &Layout::node_rotation,		// variable
	    &Layout::node_animated_part,
	    &Layout::node_billboard, // variable
	    &Layout::node_display_list,
	    &Layout::node_shadow,
	    &Layout::node_object_parent,
	    &Layout::node_generated,
	    &Layout::node_background,
	    &Layout::nop,
	    &Layout::copy_view,
	    &Layout::node_held_obj,
	    &Layout::node_scale,
	    &Layout::nop2,
	    &Layout::nop3,
	    &Layout::node_culling_radius,
	};

	enum class Opcode
	{
		branch_and_link,
		end,
		branch,
		ret,
		open_node,
		close_node,
		assign_as_view,
		update_node_flags,
		node_root,
		node_ortho_projection,
		node_perspective,
		node_start,
		node_master_list,
		node_level_of_detail,
		node_switch_case,
		node_camera,
		node_translation_rotation, // variable
		node_translation,	   // variable
		node_rotation,		   // variable
		node_animated_part,
		node_billboard, // variable
		node_display_list,
		node_shadow,
		node_object_parent,
		node_generated,
		node_background,
		nop,
		copy_view,
		node_held_obj,
		node_scale,
		nop2,
		nop3,
		node_culling_radius,
	};

	Layout::Layout(const u8* layout, XXHash64* s) : ip(layout), state(s)
	{
	}

	/*void Layout::add(const u8& n)
	{
		state->add(&n, sizeof(n));
	}

	void Layout::add(const s8& n)
	{
		state->add(&n, sizeof(n));
	}

	void Layout::add(const u16& n)
	{
		state->add(&n, sizeof(n));
	}

	void Layout::add(const s16& n)
	{
		state->add(&n, sizeof(n));
	}

	void Layout::add(const u32& n)
	{
		state->add(&n, sizeof(n));
	}

	void Layout::add(const s32& n)
	{
		state->add(&n, sizeof(n));
	}*/

	u64 Layout::fingerprint()
	{
		Opcode op;
		CommandProc func;

		while(ip != NULL)
		{
			op = (Opcode)ip[0x00];

			if((u64)op >= sizeof(jumpTable) / sizeof(jumpTable[0]))
			{
				break;
			}
			func = jumpTable[ip[0x00]];
			add(*ip);
			(this->*func)();
		}

		return state->hash();
	}

	void Layout::branch_and_link(void)
	{
		auto addr = (const GeoLayout*)cmd_ptr(0x04);
		hook::geo::fingerprint(addr, state);

		ip += CMD_PROCESS_OFFSET(8);
	}

	void Layout::end(void)
	{
		ip = nullptr;
	}

	void Layout::branch(void)
	{
		state->add(ip, 1);
		ip = (u8*)cmd_ptr(0x04);
	}

	void Layout::ret(void)
	{
		ip = nullptr;
	}

	void Layout::open_node(void)
	{
		ip += 0x04 << CMD_SIZE_SHIFT;
	}

	void Layout::close_node(void)
	{
		ip += 0x04 << CMD_SIZE_SHIFT;
	}

	void Layout::assign_as_view(void)
	{
		add(cmd_s16(0x02));

		ip += 0x04 << CMD_SIZE_SHIFT;
	}

	void Layout::update_node_flags(void)
	{
		add(cmd_u8(0x01));
		add(cmd_s16(0x02));

		ip += 0x04 << CMD_SIZE_SHIFT;
	}

	void Layout::node_root(void)
	{
		add(cmd_s16(0x02));
		add(cmd_s16(0x04));
		add(cmd_s16(0x06));
		add(cmd_s16(0x08));
		add(cmd_s16(0x0A));

		ip += 0x0C << CMD_SIZE_SHIFT;
	}

	void Layout::node_ortho_projection(void)
	{
		add(cmd_s16(0x02));
		ip += 0x04 << CMD_SIZE_SHIFT;
	}

	void Layout::node_perspective(void)
	{
		u8 flag	 = add(cmd_u8(0x01));
		s16 fov	 = add(cmd_s16(0x02));
		s16 near = add(cmd_s16(0x04));
		s16 far	 = add(cmd_s16(0x06));

		if(flag != 0)
		{
			// optional asm function
			GraphNodeFunc frustumFunc = (GraphNodeFunc)cmd_ptr(0x08);
			ip += 4 << CMD_SIZE_SHIFT;
		}

		ip += 0x08 << CMD_SIZE_SHIFT;
	}

	void Layout::node_start(void)
	{
		ip += 0x04 << CMD_SIZE_SHIFT;
	}

	void Layout::nop3(void)
	{
		ip += 0x10 << CMD_SIZE_SHIFT;
	}

	void Layout::node_master_list(void)
	{
		add(cmd_u8(0x01));
		ip += 0x04 << CMD_SIZE_SHIFT;
	}

	void Layout::node_level_of_detail(void)
	{
		add(cmd_s16(0x04));
		add(cmd_s16(0x06));
		ip += 0x08 << CMD_SIZE_SHIFT;
	}

	void Layout::node_switch_case(void)
	{
		add(cmd_s16(0x02));
		ip += 0x08 << CMD_SIZE_SHIFT;
	}

	void Layout::node_camera(void)
	{
		add(cmd_s16(0x02));

		add(cmd_s16(0x04));
		add(cmd_s16(0x06));
		add(cmd_s16(0x08));

		add(cmd_s16(0x0A));
		add(cmd_s16(0x0C));
		add(cmd_s16(0x0E));
		// auto graphNode = (GraphNodeFunc)cmd_ptr(0x10);

		ip += 0x14 << CMD_SIZE_SHIFT;
	}

	void Layout::node_translation_rotation(void)
	{
		struct GraphNodeTranslationRotation* graphNode;

		Vec3s translation, rotation;

		void* displayList = NULL;
		s16 drawingLayer  = 0;

		s16 params  = add(cmd_u8(0x01));
		s16* cmdPos = (s16*)ip;

		switch((params & 0x70) >> 4)
		{
			case 0:
				cmdPos = read_vec3s(translation, &cmdPos[2]);
				cmdPos = read_vec3s_angle(rotation, cmdPos);

				add(cmd_s16(0x04));
				add(cmd_s16(0x06));

				add(cmd_s16(0x08));
				add(cmd_s16(0x0A));
				add(cmd_s16(0x0C));
				add(cmd_s16(0x0E));
				break;
			case 1:
				cmdPos = read_vec3s(translation, &cmdPos[1]);
				add(cmd_s16(0x02));
				add(cmd_s16(0x04));
				add(cmd_s16(0x06));
				break;
			case 2:
				cmdPos = read_vec3s_angle(rotation, &cmdPos[1]);
				add(cmd_s16(0x02));
				add(cmd_s16(0x04));
				add(cmd_s16(0x06));
				break;
			case 3:
				cmdPos += 2 << CMD_SIZE_SHIFT;
				add(cmd_s16(0x02));
				break;
		}

		if(params & 0x80)
		{
			displayList  = *(void**)&cmdPos[0];
			drawingLayer = params & 0x0F;
			cmdPos += 2 << CMD_SIZE_SHIFT;
		}

		ip = (u8*)cmdPos;
	}

	void Layout::node_translation(void)
	{
		Vec3s translation;

		s16 drawingLayer  = 0;
		s16 params	  = cmd_u8(0x01);
		s16* cmdPos	  = (s16*)ip;
		void* displayList = NULL;

		cmdPos = read_vec3s(translation, &cmdPos[1]);
		add(cmd_s16(0x02));
		add(cmd_s16(0x04));
		add(cmd_s16(0x06));

		if(params & 0x80)
		{
			displayList  = *(void**)&cmdPos[0];
			drawingLayer = params & 0x0F;
			cmdPos += 2 << CMD_SIZE_SHIFT;
		}

		ip = (u8*)cmdPos;
	}

	void Layout::node_rotation(void)
	{
		Vec3s sp2c;

		s16 drawingLayer  = 0;
		s16 params	  = cmd_u8(0x01);
		s16* cmdPos	  = (s16*)ip;
		void* displayList = NULL;

		cmdPos = read_vec3s_angle(sp2c, &cmdPos[1]);
		add(cmd_s16(0x02));
		add(cmd_s16(0x04));
		add(cmd_s16(0x06));

		if(params & 0x80)
		{
			displayList  = *(void**)&cmdPos[0];
			drawingLayer = params & 0x0F;
			cmdPos += 2 << CMD_SIZE_SHIFT;
		}

		ip = (u8*)cmdPos;
	}

	void Layout::node_scale(void)
	{
		struct GraphNodeScale* graphNode;

		s16 drawingLayer  = 0;
		s16 params	  = cmd_u8(0x01);
		f32 scale	  = cmd_u32(0x04) / 65536.0f;
		void* displayList = NULL;

		add(cmd_u32(0x04));

		if(params & 0x80)
		{
			displayList  = cmd_ptr(0x08);
			drawingLayer = params & 0x0F;
			ip += 4 << CMD_SIZE_SHIFT;
		}

		ip += 0x08 << CMD_SIZE_SHIFT;
	}

	// 0x1E: No operation
	void Layout::nop2(void)
	{
		ip += 0x08 << CMD_SIZE_SHIFT;
	}

	void Layout::node_animated_part(void)
	{
		add(cmd_s16(0x02));
		add(cmd_s16(0x04));
		add(cmd_s16(0x06));

		Vec3s translation;
		s32 drawingLayer  = cmd_u8(0x01);
		void* displayList = cmd_ptr(0x08);
		s16* cmdPos	  = (s16*)ip;

		read_vec3s(translation, &cmdPos[1]);

		ip += 0x0C << CMD_SIZE_SHIFT;
	}

	void Layout::node_billboard(void)
	{
		add(cmd_s16(0x02));
		add(cmd_s16(0x04));
		add(cmd_s16(0x06));

		Vec3s translation;
		s16 drawingLayer  = 0;
		s16 params	  = cmd_u8(0x01);
		s16* cmdPos	  = (s16*)ip;
		void* displayList = NULL;

		cmdPos = read_vec3s(translation, &cmdPos[1]);

		if(params & 0x80)
		{
			displayList  = *(void**)&cmdPos[0];
			drawingLayer = params & 0x0F;
			cmdPos += 2 << CMD_SIZE_SHIFT;
		}

		ip = (u8*)cmdPos;
	}

	void Layout::node_display_list(void)
	{
		add(cmd_u8(0x1));
		auto displayList = (Gfx*)cmd_ptr(0x04);

		hook::dl::fingerprint(displayList, state);
		ip += 0x08 << CMD_SIZE_SHIFT;
	}

	void Layout::node_shadow(void)
	{
		add(cmd_s16(0x02));
		add(cmd_s16(0x04));
		add(cmd_s16(0x06));
		ip += 0x08 << CMD_SIZE_SHIFT;
	}

	void Layout::node_object_parent(void)
	{
		ip += 0x04 << CMD_SIZE_SHIFT;
	}

	void Layout::node_generated(void)
	{
		add(cmd_s16(0x02));
		ip += 0x08 << CMD_SIZE_SHIFT;
	}

	void Layout::node_background(void)
	{
		add(cmd_s16(0x02));

		// auto func = (GraphNodeFunc)cmd_ptr(0x04);

		ip += 0x08 << CMD_SIZE_SHIFT;
	}

	void Layout::nop(void)
	{
		ip += 0x08 << CMD_SIZE_SHIFT;
	}

	void Layout::copy_view(void)
	{
		add(cmd_s16(0x02));

		ip += 0x04 << CMD_SIZE_SHIFT;
	}

	void Layout::node_held_obj(void)
	{
		add(cmd_s16(0x02));
		add(cmd_s16(0x04));
		add(cmd_s16(0x06));

		struct GraphNodeHeldObject* graphNode;
		Vec3s offset;

		read_vec3s(offset, (s16*)&ip[0x02]);

		ip += 0x0C << CMD_SIZE_SHIFT;
	}

	/*
	  0x20: Create a scene graph node that specifies for an object the radius that
	   is used for frustum culling.
	   cmd+0x02: s16 cullingRadius
	*/
	void Layout::node_culling_radius(void)
	{
		add(cmd_s16(0x02));
		ip += 0x04 << CMD_SIZE_SHIFT;
	}

	static std::unordered_map<const GeoLayout*, u64>* g_layoutMap;
	static std::unordered_map<u64, const GeoLayout*>* g_idMap;

	static std::unordered_map<const GeoLayout*, u64>& layoutMap()
	{
		if(!g_layoutMap)
		{
			g_layoutMap = new std::unordered_map<const GeoLayout*, u64>();
		}

		return *g_layoutMap;
	}

	static std::unordered_map<u64, const GeoLayout*>& idMap()
	{
		if(!g_idMap)
		{
			g_idMap = new std::unordered_map<u64, const GeoLayout*>();
		}

		return *g_idMap;
	}

	void reg(const GeoLayout* layout, u64 hash)
	{
		layoutMap()[layout] = hash;
		idMap()[hash]	    = layout;
	}

	const GeoLayout* apply(const GeoLayout* func, const Id id)
	{
		u64 hash;
		if(id == Id::NONE)
		{
			try
			{
				auto hash = layoutMap().at((const GeoLayout*)func);

				auto r = sm64::asset::load(hash);

				if(r && r->isValid())
				{
					return (GeoLayout*)r->ptr();
				}
			}
			catch(...)
			{
				return func;
			}
		}
		else
		{
			auto r = sm64::asset::load((u64)id);

			if(r && r->isValid())
			{
				return (GeoLayout*)r->ptr();
			}

			try
			{
				auto func = idMap().at((u64)id);
				return func;
			}
			catch(...)
			{
			}
		}

		return func;
	}

	GeoLayout* mount(GeoLayout* func, u64 size)
	{
		return func;
		// return (GeoLayout*)asset::mount((u8*)func, size);
	}
} // namespace sm64::hook::geo

#include "script.h"
#include "engine/graph_node.h"
#include "dukglue/dukglue.h"

extern MarioBodyState gBodyStates[2];

/*
MAP THESE
MarioBodyState

FnGraphNode;
GraphNodeRoot;
GraphNodeOrthoProjection;
GraphNodePerspective;
DisplayListNode;
GraphNodeMasterList;
GraphNodeStart;
GraphNodeLevelOfDetail;
GraphNodeSwitchCase;
GraphNodeCamera; // union
GraphNodeTranslationRotation;
GraphNodeTranslation;
GraphNodeRotation;
GraphNodeAnimatedPart;
GraphNodeBillboard;
GraphNodeDisplayList;
GraphNodeScale;
GraphNodeShadow;
GraphNodeObjectParent;
GraphNodeGenerated;
GraphNodeBackground;
GraphNodeHeldObject;
*/


int myFunc(int a)
{
	return a * a;
}

class Dog {
public:
	Dog(const char* name) : name_(name)
	{
	}

	void bark()
	{
		int y = 0;
	}

	const char* getName() 
	{
		return name_.c_str();
	}

private:
	std::string name_;
};

namespace sm64::script
{
	Gfx* geo_callback(s32 callContext, GraphNode* node, Mat4* c)
	{
		return nullptr;
	}

	Gfx* geo_switch_callback(s32 callContext, GraphNodeSwitchCase* node, Mat4* c)
	{
		return nullptr;
	}

	Js::Js() : m_ctx(nullptr)
	{

	}

	Js::Js(const char* src) : m_src(src), m_ctx(nullptr)
	{
		open(src);
	}

	Js::~Js()
	{
		close();
	}

	void Js::close()
	{
		if (m_ctx)
		{
			duk_destroy_heap(m_ctx);
			m_ctx = nullptr;
		}
	}

	bool Js::open(const char* src)
	{
		if (m_ctx)
		{
			close();
		}

		if (!src)
		{
			return false;
		}

		m_ctx = duk_create_heap_default();
		m_src = src;

		dukglue_register_function(m_ctx, myFunc, "myFunc");

		dukglue_register_constructor<Dog, /* constructor args: */ const char*>(m_ctx, "Dog");
		dukglue_register_method(m_ctx, &Dog::bark, "bark");
		dukglue_register_method(m_ctx, &Dog::getName, "getName");
		return true;
	}

	s64 Js::exec()
	{
		if (!m_ctx)
		{
			return 0;
		}

		duk_peval_string(m_ctx, m_src.c_str());

		return 0;
	}
}


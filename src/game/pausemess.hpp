#pragma once
#include "controller/controllers.h"

void initiateWarp(u16 destLevel, s16 destArea, s16 destWarpNode, s32 arg3);

extern void iwaStageInit()
{
	gRedCoinsCollected = 0;
}

static void change_dialog_camera_angle()
{
	if(SpecialMode(0) == CAM_SELECTION_MARIO)
	{
		gDialogCameraAngleIndex = CAM_SELECTION_MARIO;
	}
	else
	{
		gDialogCameraAngleIndex = CAM_SELECTION_FIXED;
	}
}

static void DrawPauseBack()
{
	iTranslate(MENU_MTX_PUSH, GFX_DIMENSIONS_FROM_LEFT_EDGE(0), 240.0f, 0);

	// This is a bit weird. It reuses the dialog text box (width 130, height -80),
	// so scale to at least fit the screen.
	iScale(MENU_MTX_NOPUSH, GFX_DIMENSIONS_ASPECT_RATIO * SCREEN_HEIGHT / 130.0f, 3.0f, 1.0f);

	gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 110);
	gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

static void DrawRedCoin(s16 x, s16 y)
{
	s32 timer = frameCounter;

	iTranslate(MENU_MTX_PUSH, x, y, 0);
	iScale(MENU_MTX_NOPUSH, 0.2f, 0.2f, 1.0f);
	gDPSetRenderMode(gDisplayListHead++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);

	switch((timer / FRAME_RATE_SCALER_INV) & 6)
	{
		case 0:
			gSPDisplayList(gDisplayListHead++, coin_seg3_dl_03007940);
			break;
		case 2:
			gSPDisplayList(gDisplayListHead++, coin_seg3_dl_03007968);
			break;
		case 4:
			gSPDisplayList(gDisplayListHead++, coin_seg3_dl_03007990);
			break;
		case 6:
			gSPDisplayList(gDisplayListHead++, coin_seg3_dl_030079B8);
			break;
	}

	gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

static void DrawRedCoinNumber()
{
	s8 x;

	for(x = 0; x < gRedCoinsCollected; x++)
	{
		DrawRedCoin(GFX_DIMENSIONS_FROM_RIGHT_EDGE(30) - x * 20, 16);
	}
}

#define CRS_NUM_X1 100

#define TXT_STAR_X 98
#define ACT_NAME_X 116
#define LVL_NAME_X 117
#define MYSCORE_X 62

static void DrawPauseScene()
{
	u8 textCourse[]	 = {TEXT_COURSE};
	u8 textMyScore[] = {TEXT_MY_SCORE};

	u8 textStar[]	      = {TEXT_STAR};
	u8 textUnfilledStar[] = {TEXT_UNFILLED_STAR};

	u8 strCourseNum[4];
	void** courseNameTbl;
	u8* courseName;
	void** actNameTbl;
	u8* actName;
	u8 courseIndex;
	u8 starFlags;

	courseNameTbl = (void**)segmented_to_virtual(seg2_course_name_table);
	actNameTbl    = (void**)segmented_to_virtual(seg2_act_name_table);

	courseIndex = activeCourseNo - 1;
	starFlags   = BuGetStarFlag(activePlayerNo - 1, activeCourseNo - 1);

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);

	if(courseIndex < COURSE_STAGES_COUNT)
	{
		DrawMyScore(1, activePlayerNo - 1, courseIndex, 178, 103);
		DrawCourseStar(activePlayerNo - 1, courseIndex, 118, 103);
	}

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);

	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);

	if(courseIndex < COURSE_STAGES_COUNT && BuGetNumStars(activePlayerNo - 1, courseIndex) != 0)
	{
		Draw8bitFont(MYSCORE_X, 121, textMyScore);
	}

	courseName = (u8*)segmented_to_virtual(courseNameTbl[courseIndex]);

	if(courseIndex < COURSE_STAGES_COUNT)
	{
		Draw8bitFont(63, 157, textCourse);

		I_itochar(activeCourseNo, strCourseNum);
		Draw8bitFont(CRS_NUM_X1, 157, strCourseNum);

		actName = (u8*)segmented_to_virtual(actNameTbl[(activeCourseNo - 1) * 6 + gDialogCourseActNum - 1]);

		if(starFlags & (1 << (gDialogCourseActNum - 1)))
		{
			Draw8bitFont(TXT_STAR_X, 140, textStar);
		}
		else
		{
			Draw8bitFont(TXT_STAR_X, 140, textUnfilledStar);
		}
		Draw8bitFont(ACT_NAME_X, 140, actName);
		Draw8bitFont(LVL_NAME_X, 157, &courseName[3]);
	}
	else
	{
		Draw8bitFont(94, 157, &courseName[3]);
	}
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

#define TXT1_X 3
#define TXT2_X 119
#define Y_VAL7 2

static void DrawCameraChange(s16 x, s16 y, s8* index, s16 xIndex)
{
	u8 textLakituMario[] = {TEXT_LAKITU_MARIO};
	u8 textLakituStop[]  = {TEXT_LAKITU_STOP};

	u8 textNormalUpClose[] = {TEXT_NORMAL_UPCLOSE};
	u8 textNormalFixed[]   = {TEXT_NORMAL_FIXED};

	ContCursorEvent(MENU_SCROLL_HORIZONTAL, index, 1, 2);

	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);

	Draw8bitFont(x + 14, y + 2, textLakituMario);
	Draw8bitFont(x + TXT1_X, y - 13, textNormalUpClose);
	Draw8bitFont(x + 124, y + 2, textLakituStop);
	Draw8bitFont(x + TXT2_X, y - 13, textNormalFixed);

	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
	iTranslate(MENU_MTX_PUSH, ((index[0] - 1) * xIndex) + x, y + Y_VAL7, 0);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	switch(index[0])
	{
		case 1:
			SpecialMode(1);
			break;
		case 2:
			SpecialMode(2);
			break;
	}
}

#define X_VAL8 4
#define Y_VAL8 2

static void DrawContinue(s16 x, s16 y, s8* index, s16 yIndex)
{
	u8 textContinue[]     = {TEXT_CONTINUE};
	u8 textExitCourse[]   = {TEXT_EXIT_COURSE};
	u8 textCameraAngleR[] = {TEXT_CAMERA_ANGLE_R};

	ContCursorEvent(MENU_SCROLL_VERTICAL, index, 1, 3);

	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);

	Draw8bitFont(x + 10, y - 2, textContinue);
	Draw8bitFont(x + 10, y - 17, textExitCourse);

	DrawPauseScore();

	if(index[0] != 3)
	{
		Draw8bitFont(x + 10, y - 33, textCameraAngleR);
		gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

		iTranslate(MENU_MTX_PUSH, x - X_VAL8, (y - ((index[0] - 1) * yIndex)) - Y_VAL8, 0);

		gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);
		gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
		gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
	}

	if(index[0] == 3)
	{
		DrawCameraChange(x - 42, y - 42, &gDialogCameraAngleIndex, 110);
	}
}

namespace sm64::hud
{
	class String
	{
		public:
		/*String()
		{
		}*/

		String(const std::string& str)
		{
			for(auto& c : str)
			{
				if(c >= 'A' && c <= 'Z')
				{
					m_buffer.push_back(c + 0x0A - 'A');
				}
				else if(c >= 'a' && c <= 'z')
				{
					m_buffer.push_back(c + 0x0A - 'a');
				}
				else if(c >= '0' && c <= '9')
				{
					m_buffer.push_back(c - '0');
				}
				else if(c == ' ')
				{
					m_buffer.push_back(0x9E);
				}
				else if(c == '.')
				{
					m_buffer.push_back(0x24);
				}
				else
				{
					m_buffer.push_back(0x00);
				}
			}

			m_buffer.push_back(0xFF);
		}

		const u8* buffer() const
		{
			return m_buffer.data();
		}

		protected:
		std::vector<u8> m_buffer;
	};
} // namespace sm64::hud

static void print_hud_colorful_str(const sm64::hud::String& str, s16 x, s16 y)
{
	u8 textPause[] = {TEXT_PAUSE};

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);

	Draw16bitFont(HUD_LUT_GLOBAL, x, y, str.buffer());

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

sm64::hud::String R_NEXT = std::string("R");
sm64::hud::String R_BACK = std::string("BACK");

namespace sm64::menu::item
{
	class Base
	{
		public:
		Base(const std::string& title, const std::function<void(s64)>& onChange) : m_title(title), m_titleEncoded(title), m_onChange(onChange)
		{
		}

		virtual ~Base()
		{
		}

		virtual void draw(const s16 x, const s16 y)
		{
			Draw8bitFont(x, y, m_titleEncoded.buffer());
		}

		virtual void click()
		{
			if (m_onChange)
			{
				m_onChange(0);
			}
		}

		const char* title() const
		{
			return m_title.c_str();
		}
		const u8* titleEncoded() const
		{
			return m_titleEncoded.buffer();
		}

		protected:
		std::string m_title;
		hud::String m_titleEncoded;
		std::function<void(s64)> m_onChange;
	};

	class EnumFloat : public Base
	{
		public:
		EnumFloat(const std::string& title, const std::vector<float>& enums, float* value) : Base(title, nullptr), m_value(value), m_index(0)
		{
			char buffer[32] = {};
			for(auto f : enums)
			{
				m_enums.push_back(f);
				sprintf(buffer, "%2.2f", f);
				m_labels.push_back(hud::String(buffer));
			}
		}

		void draw(const s16 x, const s16 y) override
		{
			Base::draw(x, y);

			if(m_value)
			{
				u32 i = 0;
				for(auto v : m_enums)
				{
					if(v == *m_value)
					{
						Draw8bitFont(x + 110, y, m_labels[i].buffer());
						break;
					}
					i++;
				}
			}
		}

		void click() override
		{
			m_index = (m_index + 1) % m_enums.size();

			if(m_value && m_index < m_enums.size())
			{
				*m_value = m_enums[m_index];
			}
		}

		protected:
		std::vector<float> m_enums;
		std::vector<hud::String> m_labels;
		float* m_value;
		u32 m_index;
	};

	class Bool : public Base
	{
		public:
		Bool(const std::string& title, bool* value, const std::function<void(bool)>& onChange = nullptr) : Base(title, nullptr), m_value(value), m_trueString("ON"), m_falseString("OFF"), m_onBoolChange(onChange)
		{
			m_onChange = [this](s64 v) {
				if(m_value)
				{
					*m_value = v != 0;

					if(m_onBoolChange)
					{
						m_onBoolChange(*m_value);
					}
				}
			};
		}

		void draw(const s16 x, const s16 y) override
		{
			Base::draw(x, y);

			if(m_value)
			{
				if(*m_value)
				{
					Draw8bitFont(x + 110, y, m_trueString.buffer());
				}
				else
				{
					Draw8bitFont(x + 110, y, m_falseString.buffer());
				}
			}
		}

		void click() override
		{
			if(m_value)
			{
				*m_value = !*m_value;

				if(m_onBoolChange)
				{
					m_onBoolChange(*m_value);
				}
			}
		}

		protected:
		bool* m_value;
		hud::String m_trueString;
		hud::String m_falseString;
		std::function<void(bool)> m_onBoolChange;
	};
} // namespace sm64::menu::item

namespace sm64::menu
{
#define DIALOG_TITLE_X 123
#define DIALOG_TITLE_Y 61

	class Dialog
	{
		public:
		Dialog(const char* title = "") : m_index(1), title(title)
		{
		}

		virtual ~Dialog()
		{
		}

		virtual void render(const s16 x, const s16 y, const s16 yIndex = 15)
		{
			ContCursorEvent(MENU_SCROLL_VERTICAL, &m_index, 1, m_items.size());

			if(sm64::player(0).controller().buttonPressed & sm64::hid::A_BUTTON)
			{
				if(m_index && m_index - 1 < m_items.size())
				{
					m_items[m_index - 1]->click();
				}

				sm64::config().save();
			}

			gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
			gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);

			s32 yOffset = 2;
			for(u32 i = 0; i < m_items.size(); i++)
			{
				auto& item = m_items[i];
				item->draw(x + 10, y - yOffset);
				yOffset += 15;
			}

			DrawPauseScore();

			// Draw8bitFont(x + 10, y - 33, textCameraAngleR);
			gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

			iTranslate(MENU_MTX_PUSH, x - X_VAL8, (y - ((m_index - 1) * yIndex)) - Y_VAL8, 0);

			gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);
			gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
			gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
		}

		hud::String title;

		protected:
		std::vector<item::Base*> m_items;
		s8 m_index;
	};

	class Cheats : public Dialog
	{
		public:
		Cheats() : Dialog("CHEATS")
		{
			if (sm64::config().cheats().speed() < 1.0f)
			{
				sm64::config().cheats().speed() = 1.0f;
			}

			m_items.push_back(new sm64::menu::item::Bool("UNLIMITED LIVES", &sm64::config().cheats().unlimitedLives()));
			m_items.push_back(new sm64::menu::item::Bool("INVINCIBLE", &sm64::config().cheats().invincible()));
			m_items.push_back(new sm64::menu::item::EnumFloat("SPEED", { 1.0f, 1.25f, 1.5f, 2.0f, 3.0f }, &sm64::config().cheats().speed()));
			m_items.push_back(new sm64::menu::item::Bool("MOON JUMP", &sm64::config().cheats().moonJump()));
			m_items.push_back(new sm64::menu::item::Bool("SUPER JUMP", &sm64::config().cheats().superJump()));
			m_items.push_back(new sm64::menu::item::Bool("QUADRUPLE JUMP", &sm64::config().cheats().quadrupleJump()));
			m_items.push_back(new sm64::menu::item::EnumFloat("BOWSER AIM ASSIST", {0.0f, 12.5f, 25.0f, 45.0f, 60.0f}, &sm64::config().cheats().bowserAimAssist()));
		}

		void render(const s16 x, const s16 y, const s16 yIndex = 15) override
		{
			print_hud_colorful_str(std::string("CHEATS"), DIALOG_TITLE_X, DIALOG_TITLE_Y);
			Dialog::render(x, y, yIndex);
		}
	};

	

	template<int input>
	void rebindCallback(u64 value)
	{
		sm64::player(0).rebind(input);
	}

	class Rebind : public Dialog
	{
	public:
		Rebind() : Dialog("REBIND")
		{
			m_items.push_back(new sm64::menu::item::Base("A BUTTON", rebindCallback<hid::A_BUTTON>));
			m_items.push_back(new sm64::menu::item::Base("B BUTTON", rebindCallback<hid::B_BUTTON>));
			m_items.push_back(new sm64::menu::item::Base("Z TRIG", rebindCallback<hid::Z_TRIG>));
			m_items.push_back(new sm64::menu::item::Base("U CBUTTONS", rebindCallback<hid::U_CBUTTONS>));
			m_items.push_back(new sm64::menu::item::Base("L CBUTTONS", rebindCallback<hid::L_CBUTTONS>));
			m_items.push_back(new sm64::menu::item::Base("D CBUTTONS", rebindCallback<hid::D_CBUTTONS>));
			m_items.push_back(new sm64::menu::item::Base("R CBUTTONS",  rebindCallback<hid::R_CBUTTONS>));
			m_items.push_back(new sm64::menu::item::Base("R TRIG", rebindCallback<hid::R_TRIG>));
			m_items.push_back(new sm64::menu::item::Base("WALK", rebindCallback<hid::WALK_BUTTON>));
			m_items.push_back(new sm64::menu::item::Base("UP", rebindCallback<hid::STICK_X_UP>));
			m_items.push_back(new sm64::menu::item::Base("RIGHT", rebindCallback<hid::STICK_X_RIGHT>));
			m_items.push_back(new sm64::menu::item::Base("DOWN", rebindCallback<hid::STICK_X_DOWN>));
			m_items.push_back(new sm64::menu::item::Base("LEFT", rebindCallback<hid::STICK_X_LEFT>));
			m_items.push_back(new sm64::menu::item::Base("START BUTTON", rebindCallback<hid::START_BUTTON>));
		}

		void render(const s16 x, const s16 y, const s16 yIndex = 15) override
		{
			static const int offset = 50;
			print_hud_colorful_str(std::string("REBIND"), DIALOG_TITLE_X, DIALOG_TITLE_Y - offset);

			if (sm64::player(0).isRebindMode())
			{
				static hud::String pressKey =std::string("PRESS KEY TO BIND");

				ContCursorEvent(MENU_SCROLL_VERTICAL, &m_index, 1, m_items.size());

				gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
				gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);

				Draw8bitFont(x + 10, y - 30, pressKey.buffer());

				DrawPauseScore();

				// Draw8bitFont(x + 10, y - 33, textCameraAngleR);
				gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

				iTranslate(MENU_MTX_PUSH, x - X_VAL8, (y - ((m_index - 1) * yIndex)) - Y_VAL8, 0);

				gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);
				gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
				gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
			}
			else
			{
				Dialog::render(x, y + offset, yIndex);
			}
		}
	};


	template<int input, int course>
	void levelSelectCallback(u64 value)
	{
		initiateWarp(input, 0x01, 0x0A, 0);
	}

	class LevelSelect : public Dialog
	{
	public:
		LevelSelect() : Dialog("LEVEL SELECT")
		{
			m_items.push_back(new sm64::menu::item::Base("BOBOMB BATTLEFIELD", levelSelectCallback<LEVEL_BOB, COURSE_BOB>));
			m_items.push_back(new sm64::menu::item::Base("SNOWMANS LAND", levelSelectCallback<LEVEL_SL, COURSE_SL>));
			m_items.push_back(new sm64::menu::item::Base("WHOMPS FORTRESS", levelSelectCallback<LEVEL_WF, COURSE_SL>));
		}

		void render(const s16 x, const s16 y, const s16 yIndex = 15) override
		{
			print_hud_colorful_str(std::string("LEVEL SELECT"), DIALOG_TITLE_X, DIALOG_TITLE_Y);

			Dialog::render(x, y, yIndex);
		}
	};

	class Mods : public Dialog
	{
		public:
		Mods() : Dialog("MODIFICATIONS")
		{
			for(auto& dir : config().mods().dirs())
			{
				std::string name = dir.first.c_str() + ((dir.first[0] < '0' || dir.first[0] == '~') ? 1 : 0);
				std::transform(name.begin(), name.end(), name.begin(), ::toupper);
				m_items.push_back(new sm64::menu::item::Bool(name, &dir.second));
			}
		}

		void render(const s16 x, const s16 y, const s16 yIndex = 15) override
		{
			print_hud_colorful_str(std::string("MODS"), DIALOG_TITLE_X, DIALOG_TITLE_Y);
			Dialog::render(x, y, yIndex);
		}
	};

	class Camera : public Dialog
	{
		public:
		Camera() : Dialog("CAMERA")
		{
			// m_items.push_back(new sm64::menu::item::EnumFloat("DISTANCE SCALER", {1.0f, 1.25f, 1.5f, 2.0f, 3.0f}, &sm64::config().camera().distanceScaler()));

			m_items.push_back(new sm64::menu::item::Bool("DISABLE DIST CLIP", &sm64::config().camera().setDisableDistanceClip()));
			m_items.push_back(new sm64::menu::item::Bool("CLASSIC CAMERA", &sm64::config().camera().useClassicCamera()));
			m_items.push_back(new sm64::menu::item::EnumFloat("RETURN SCALER", {1.0f, 0.75f, 0.5f, 0.25f, 0.10f, 0.0f}, &sm64::config().camera().yawReturnScaler()));

#ifdef ENABLE_MOUSE
			m_items.push_back(new sm64::menu::item::EnumFloat("MOUSE X SCALER", {4.0f, 5.0f, 6.0f, 1.0f, 2.0f, 3.0f}, &sm64::config().camera().mousexScaler()));
			m_items.push_back(new sm64::menu::item::EnumFloat("MOUSE Y SCALER", {4.0f, 5.0f, 6.0f, 1.0f, 2.0f, 3.0f }, &sm64::config().camera().mouseyScaler()));
			m_items.push_back(new sm64::menu::item::Bool("MOUSE X INVERT", &sm64::config().camera().mousexInvert()));
			m_items.push_back(new sm64::menu::item::Bool("MOUSE Y INVERT", &sm64::config().camera().mouseyInvert()));
#endif
		}

		void render(const s16 x, const s16 y, const s16 yIndex = 15) override
		{
			print_hud_colorful_str(std::string("CAMERA"), DIALOG_TITLE_X, DIALOG_TITLE_Y);
			Dialog::render(x, y, yIndex);
		}
	};

	class Game : public Dialog
	{
		public:
		Game() : Dialog("GAME")
		{
#ifndef __SWITCH__
			m_items.push_back(new sm64::menu::item::Bool("FULL SCREEN", &sm64::config().game().fullscreen(), [](bool value) { sm64::gfx::set_fullscreen(value); }));
#endif
			m_items.push_back(new sm64::menu::item::Bool("NO STAR EXIT", &sm64::config().game().setNoStarExit()));
			m_items.push_back(new sm64::menu::item::Bool("NO FRAME PACING", &sm64::config().game().disableFramePacing()));
			m_items.push_back(new sm64::menu::item::Bool("MIRROR MODE", &sm64::config().game().setMirror()));
			m_items.push_back(new sm64::menu::item::Bool("RECORD TAS", &sm64::config().game().recordTas()));
			m_items.push_back(new sm64::menu::item::Bool("DISABLE SOUND", &sm64::config().game().disableSound()));
#ifdef ENABLE_MOUSE
			m_items.push_back(new sm64::menu::item::Bool("FORCE MOUSE", &sm64::config().game().forceMouse()));
#endif
		}

		void render(const s16 x, const s16 y, const s16 yIndex = 15) override
		{
			print_hud_colorful_str(std::string("GAME"), DIALOG_TITLE_X, DIALOG_TITLE_Y);
			Dialog::render(x, y, yIndex);
		}
	};

	class Credits : public Dialog
	{
		public:
		Credits() : Dialog("CREDITS")
		{
		}

		void render(const s16 x, const s16 y, const s16 yIndex = 15) override
		{
			static std::vector<hud::String> credits = {std::string("SM64 DECOMP TEAM"),	std::string("ARREDONDO 3D MODELS"),	    std::string("SGI HD MODELS"), std::string("PIG REXXER MODELS"),
								   std::string("HYPATIA TEXTURE PACK"), std::string("CLEANER AESTHETICS TEXTURES"), std::string("MANY MORE")};
			print_hud_colorful_str(std::string("CREDITS"), DIALOG_TITLE_X, DIALOG_TITLE_Y);

			gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
			gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);

			u32 yOffset = 2;
			for(u32 i = 0; i < credits.size(); i++)
			{
				Draw8bitFont(x + 10, y - yOffset, credits[i].buffer());
				yOffset += 15;
			}

			DrawPauseScore();

			// Draw8bitFont(x + 10, y - 33, textCameraAngleR);
			gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

			iTranslate(MENU_MTX_PUSH, x - X_VAL8, (y - ((m_index - 1) * yIndex)) - Y_VAL8, 0);

			gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);
			gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
			gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
		}
	};
} // namespace sm64::menu

static void DrawScoreWin(s16 x, s16 y)
{
	const s16 xOff = 78;
	const s16 yOff = 32;

	iTranslate(MENU_MTX_PUSH, x - xOff, y - yOff, 0);
	iScale(MENU_MTX_NOPUSH, 1.2f, 0.8f, 1.0f);
	gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 105);
	gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	iTranslate(MENU_MTX_PUSH, x + 6, y - 28, 0);
	iRotate(MENU_MTX_NOPUSH, DEFAULT_DIALOG_BOX_ANGLE, 0, 0, 1.0f);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	iTranslate(MENU_MTX_PUSH, x - 9, y - 101, 0);
	iRotate(MENU_MTX_NOPUSH, 270.0f, 0, 0, 1.0f);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

static void SelectRoomPauseInit()
{
	u8 courseDone;

	if(gLastCompletedCourseNum == 0)
	{
		courseDone = 0;
	}
	else
	{
		courseDone = gLastCompletedCourseNum - 1;

		if(courseDone >= COURSE_STAGES_COUNT)
		{
			courseDone = COURSE_STAGES_COUNT;
		}
	}

	gDialogLineNum = courseDone;
}

static void DrawOutOfPauseScene()
{
	u8 textPause[] = {TEXT_PAUSE};

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);

	Draw16bitFont(HUD_LUT_GLOBAL, 123, 81, textPause);

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

static void DrawScoreStar(s16 x, s16 y, s16 fileNum, s16 courseNum)
{
	s16 hasStar = 0;

	u8 str[COURSE_STAGES_COUNT * 2];

	u8 textStar[] = {TEXT_STAR};

	u8 starFlags  = BuGetStarFlag(fileNum, courseNum);
	u16 starCount = BuGetNumStars(fileNum, courseNum);

	u16 nextStar = 0;

	if(starFlags & 0x40)
	{
		starCount--;
		Draw8bitFont(x + 89, y - 5, textStar);
	}

	while(hasStar != starCount)
	{
		if(starFlags & (1 << nextStar))
		{
			str[nextStar * 2] = DIALOG_CHAR_STAR_FILLED;
			hasStar++;
		}
		else
		{
			str[nextStar * 2] = DIALOG_CHAR_STAR_OPEN;
		}

		str[nextStar * 2 + 1] = DIALOG_CHAR_SPACE;
		nextStar++;
	}

	if(starCount == nextStar && starCount != 6)
	{
		str[nextStar * 2]     = DIALOG_CHAR_STAR_OPEN;
		str[nextStar * 2 + 1] = DIALOG_CHAR_SPACE;
		nextStar++;
	}

	str[nextStar * 2] = DIALOG_CHAR_TERMINATOR;

	Draw8bitFont(x + 14, y + 13, str);
}

static void render_pause_castle_main_strings(s16 x, s16 y)
{
	void** courseNameTbl = (void**)segmented_to_virtual(seg2_course_name_table);

	u8 textCoin[] = {TEXT_COIN_X};

	void* courseName;

	u8 strVal[8];
	s16 starNum = gDialogLineNum;

	ContCursorEvent(MENU_SCROLL_VERTICAL, &gDialogLineNum, -1, COURSE_STAGES_COUNT + 1);

	if(gDialogLineNum == COURSE_STAGES_COUNT + 1)
	{
		gDialogLineNum = 0;
	}

	if(gDialogLineNum == -1)
	{
		gDialogLineNum = COURSE_STAGES_COUNT;
	}

	if(gDialogLineNum != COURSE_STAGES_COUNT)
	{
		while(BuGetNumStars(activePlayerNo - 1, gDialogLineNum) == 0)
		{
			if(gDialogLineNum >= starNum)
			{
				gDialogLineNum++;
			}
			else
			{
				gDialogLineNum--;
			}

			if(gDialogLineNum == COURSE_STAGES_COUNT || gDialogLineNum == -1)
			{
				gDialogLineNum = COURSE_STAGES_COUNT;
				break;
			}
		}
	}

	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);

	if(gDialogLineNum < COURSE_STAGES_COUNT)
	{
		courseName = segmented_to_virtual(courseNameTbl[gDialogLineNum]);
		DrawScoreStar(x, y, activePlayerNo - 1, gDialogLineNum);
		Draw8bitFont(x + 34, y - 5, textCoin);

		I_itochar(BuGetNumCoins(activePlayerNo - 1, gDialogLineNum), strVal);
		Draw8bitFont(x + 54, y - 5, strVal);
	}
	else
	{
		u8 textStarX[] = {TEXT_STAR_X};
		courseName     = segmented_to_virtual(courseNameTbl[COURSE_MAX]);
		Draw8bitFont(x + 40, y + 13, textStarX);
		I_itochar(BuGetSumStars(activePlayerNo - 1, COURSE_BONUS_STAGES - 1, COURSE_MAX - 1), strVal);
		Draw8bitFont(x + 60, y + 13, strVal);
	}

	Draw8bitFont(x - 9, y + 30, (const u8*)courseName);
	DrawPauseScore();

	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

s8 gCourseCompleteCoinsEqual = 0;
s32 saveScene_counter	     = 0;
s32 gCourseCompleteCoins     = 0;
s8 getStar_flag		     = 0;
s8 subMenu		     = 0;

static std::vector<sm64::menu::Dialog*>& menus()
{
	static std::vector<sm64::menu::Dialog*> sMenus = {new sm64::menu::Game(), new sm64::menu::Camera(), new sm64::menu::Mods(), new sm64::menu::Cheats(), new sm64::menu::Rebind(), /*new sm64::menu::LevelSelect(),*/ new sm64::menu::Credits()};
	return sMenus;
}

static void DrawPauseScore()
{
	s16 x = 0;
	s16 y = 10;

	Draw8bitFont(x, y, R_NEXT.buffer());

	if(subMenu >= menus().size())
	{
		Draw8bitFont(x + 15, y, R_BACK.buffer());
	}
	else
	{
		Draw8bitFont(x + 15, y, menus()[subMenu]->title.buffer());
	}
}

extern s16 PauseSceneEvent()
{
	s16 num;

	if(sm64::player(0).controller().buttonPressed & sm64::hid::R_TRIG)
	{
		subMenu++;
		subMenu = subMenu % (menus().size() + 1);
	}
	else if(sm64::player(0).controller().buttonPressed & sm64::hid::L_TRIG)
	{
		subMenu--;
		subMenu = subMenu % (menus().size() + 1);
	}

	switch(gDialogBoxState)
	{
		case DIALOG_STATE_OPENING:
			gDialogLineNum = 1;
			font_alpha     = 0;
			FreezeGame(-1, 0);

			AudStartSound(SOUND_MENU_PAUSE_HIGHPRIO, gDefaultSoundArgs);

			if(activeCourseNo >= COURSE_MIN && activeCourseNo <= COURSE_MAX)
			{
				change_dialog_camera_angle();
				gDialogBoxState = DIALOG_STATE_VERTICAL;
			}
			else
			{
				SelectRoomPauseInit();
				gDialogBoxState = DIALOG_STATE_HORIZONTAL;
			}
			break;
		case DIALOG_STATE_VERTICAL:
			DrawPauseBack();

			if(playerWorks[0].status & ACT_FLAG_PAUSE_EXIT)
			{
				if(!subMenu)
				{
					DrawPauseScene();
					DrawRedCoinNumber();
					DrawContinue(99, 93, &gDialogLineNum, 15);
				}
				else if(subMenu - 1 < menus().size())
				{
					menus()[subMenu - 1]->render(99, 93 + 40);
				}
			}
			else if(subMenu && subMenu - 1 < menus().size())
			{
				menus()[subMenu - 1]->render(99, 93 + 40);
			}
			else
			{
				DrawPauseScene();
				DrawRedCoinNumber();
			}

			if((sm64::player(0).controller().buttonPressed & sm64::hid::A_BUTTON && subMenu == 0) || sm64::player(0).controller().buttonPressed & sm64::hid::START_BUTTON)
			{
				FreezeGame(0, 0);
				AudStartSound(SOUND_MENU_PAUSE_2, gDefaultSoundArgs);
				gDialogBoxState = DIALOG_STATE_OPENING;
				gMenuMode	= -1;

				if(gDialogLineNum == 2)
				{
					num = gDialogLineNum;
				}
				else
				{
					num = 1;
				}

				return num;
			}
			break;
		case DIALOG_STATE_HORIZONTAL:
			DrawPauseBack();

			if(!subMenu)
			{
				DrawOutOfPauseScene();
				DrawScoreWin(160, 143);
				render_pause_castle_main_strings(104, 60);
			}
			else if(subMenu - 1 < menus().size())
			{
				menus()[subMenu - 1]->render(99, 93 + 40);
			}

			if((sm64::player(0).controller().buttonPressed & sm64::hid::A_BUTTON && subMenu == 0) || sm64::player(0).controller().buttonPressed & sm64::hid::START_BUTTON)
			{
				FreezeGame(0, 0);
				AudStartSound(SOUND_MENU_PAUSE_2, gDefaultSoundArgs);
				gMenuMode	= -1;
				gDialogBoxState = DIALOG_STATE_OPENING;

				return 1;
			}
			break;
	}

	if(font_alpha < 250)
	{
		font_alpha += 25 / FRAME_RATE_SCALER_INV;
	}

	return 0;
}

#define TXT_HISCORE_X 109
#define TXT_HISCORE_Y 36
#define TXT_CONGRATS_X 70

#define HUD_PRINT_HISCORE 0
#define HUD_PRINT_CONGRATULATIONS 1

static void SpecialDemo(s8 str)
{
	u8 textHiScore[]	 = {TEXT_HUD_HI_SCORE};
	u8 textCongratulations[] = {TEXT_HUD_CONGRATULATIONS};

	u8 colorFade = sins(gDialogColorFadeTimer) * 50.0f + 200.0f;

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, colorFade, colorFade, colorFade, 255);

	if(str == HUD_PRINT_HISCORE)
	{
		Draw16bitFont(HUD_LUT_GLOBAL, TXT_HISCORE_X, TXT_HISCORE_Y, textHiScore);
	}
	else
	{
		Draw16bitFont(HUD_LUT_GLOBAL, TXT_CONGRATS_X, 67, textCongratulations);
	}

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

static void CoinCountDemo(s16 x, s16 y)
{
	u8 courseCompleteCoinsStr[4];
	u8 hudTextSymCoin[] = {GLYPH_COIN, GLYPH_SPACE};
	u8 hudTextSymX[]    = {GLYPH_MULTIPLY, GLYPH_SPACE};

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);

	Draw16bitFont(HUD_LUT_GLOBAL, x, y, hudTextSymCoin);
	Draw16bitFont(HUD_LUT_GLOBAL, x + 16, y, hudTextSymX);

	I_itochar(gCourseCompleteCoins, courseCompleteCoinsStr);
	Draw16bitFont(HUD_LUT_GLOBAL, x + 32, y, courseCompleteCoinsStr);

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

	if(gCourseCompleteCoins >= playerMeter.coins)
	{
		gCourseCompleteCoinsEqual = 1;
		gCourseCompleteCoins	  = playerMeter.coins;

		if(gGotFileCoinHiScore != 0)
		{
			SpecialDemo(HUD_PRINT_HISCORE);
		}
	}
	else
	{
		if((saveScene_counter & 1) || playerMeter.coins > 70)
		{
			gCourseCompleteCoins++;
			AudStartSound(SOUND_MENU_YOSHI_GAIN_LIVES, gDefaultSoundArgs);

			if(gCourseCompleteCoins == 50 || gCourseCompleteCoins == 100 || gCourseCompleteCoins == 150)
			{
				AudStartSound(SOUND_GENERAL_COLLECT_1UP, gDefaultSoundArgs);
				if(!sm64::config().cheats().unlimitedLives())
				{
					marioWorks[0].numLives++;
				}
			}
		}

		if(playerMeter.coins == gCourseCompleteCoins && gGotFileCoinHiScore != 0)
		{
			AudStartSound(SOUND_MENU_MARIO_CASTLE_WARP2, gDefaultSoundArgs);
		}
	}
}

static void StarGetEffect(s32 arg, u8 starNum)
{
	if(playerMeter.coins == gCourseCompleteCoins && (gCurrCourseStarFlags & starNum) == 0 && getStar_flag == 0)
	{
		Na_HiScoreBgm();
		getStar_flag = arg;
	}
}

#define TXT_NAME_X1 71
#define TXT_NAME_X2 69

#define CRS_NUM_X2 104
#define CRS_NUM_X3 102
#define TXT_CLEAR_X1 MessageLength(name) + 81
#define TXT_CLEAR_X2 MessageLength(name) + 79

static void DrawSaveDemo()
{
	u8 textCourse[]	      = {TEXT_COURSE};
	UNUSED u8 textCatch[] = {TEXT_CATCH}; // size in US
	UNUSED u8 textClear[] = {TEXT_CLEAR};
	u8 textSymStar[]      = {GLYPH_STAR, GLYPH_SPACE};

	void** actNameTbl;
	void** courseNameTbl;
	u8* name;

	u8 strCourseNum[4];

	actNameTbl    = (void**)seg2_act_name_table;
	courseNameTbl = (void**)seg2_course_name_table;

	if(gLastCompletedCourseNum <= COURSE_STAGES_MAX)
	{
		CoinCountDemo(118, 103);
		StarGetEffect(1, 1 << (gLastCompletedStarNum - 1));

		if(gLastCompletedStarNum == 7)
		{
			name = (u8*)actNameTbl[COURSE_STAGES_MAX * 6 + 1];
		}
		else
		{
			name = (u8*)actNameTbl[(gLastCompletedCourseNum - 1) * 6 + gLastCompletedStarNum - 1];
		}

		gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
		I_itochar(gLastCompletedCourseNum, strCourseNum);
		gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, font_alpha);
		Draw8bitFont(65, 165, textCourse);
		Draw8bitFont(CRS_NUM_X2, 165, strCourseNum);
		gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);
		Draw8bitFont(63, 167, textCourse);
		Draw8bitFont(CRS_NUM_X3, 167, strCourseNum);
		gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
	}
	else if(gLastCompletedCourseNum == COURSE_BITDW || gLastCompletedCourseNum == COURSE_BITFS)
	{
		name = (u8*)courseNameTbl[gLastCompletedCourseNum - 1];
		gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
		gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, font_alpha);

		Draw8bitFont(TXT_NAME_X1, 130, name);

		Draw8bitFont(TXT_CLEAR_X1, 130, textClear);

		gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);
		Draw8bitFont(TXT_NAME_X2, 132, name);
		Draw8bitFont(TXT_CLEAR_X2, 132, textClear);

		gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
		SpecialDemo(HUD_PRINT_CONGRATULATIONS);
		CoinCountDemo(118, 111);
		StarGetEffect(2, 0); //! 2 isn't defined, originally for key hud?
		return;
	}
	else
	{
		name = (u8*)actNameTbl[COURSE_STAGES_MAX * 6];
		CoinCountDemo(118, 103);
		StarGetEffect(1, 1 << (gLastCompletedStarNum - 1));
	}

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);
	Draw16bitFont(HUD_LUT_GLOBAL, 55, 77, textSymStar);
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, font_alpha);
	Draw8bitFont(76, 145, name);

	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);
	Draw8bitFont(74, 147, name);

	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

#define TXT_SAVEOPTIONS_X x + 12
#define TXT_SAVECONT_Y 0
#define TXT_SAVEQUIT_Y 20
#define TXT_CONTNOSAVE_Y 40

#define X_VAL9 x
static void SaveCheckContinue(s16 x, s16 y, s8* index, s16 sp6e)

{
	u8 textSaveAndContinue[]     = {TEXT_SAVE_AND_CONTINUE};
	u8 textSaveAndQuit[]	     = {TEXT_SAVE_AND_QUIT};
	u8 textContinueWithoutSave[] = {TEXT_CONTINUE_WITHOUT_SAVING};

	ContCursorEvent(MENU_SCROLL_VERTICAL, index, 1, 3);

	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);

	Draw8bitFont(TXT_SAVEOPTIONS_X, y + TXT_SAVECONT_Y, textSaveAndContinue);
	Draw8bitFont(TXT_SAVEOPTIONS_X, y - TXT_SAVEQUIT_Y, textSaveAndQuit);
	Draw8bitFont(TXT_SAVEOPTIONS_X, y - TXT_CONTNOSAVE_Y, textContinueWithoutSave);

	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

	iTranslate(MENU_MTX_PUSH, X_VAL9, y - ((index[0] - 1) * sp6e), 0);

	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, font_alpha);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);

	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

extern s16 SaveSceneEvent()
{
	s16 num;

	switch(gDialogBoxState)
	{
		case DIALOG_STATE_OPENING:
			DrawSaveDemo();
			if(saveScene_counter > 100 && gCourseCompleteCoinsEqual == 1)
			{
				gDialogBoxState = DIALOG_STATE_VERTICAL;
				FreezeGame(-1, 0);
				font_alpha     = 0;
				gDialogLineNum = 1;
			}
			break;
		case DIALOG_STATE_VERTICAL:
			DrawPauseBack();
			DrawSaveDemo();

			SaveCheckContinue(100, 86, &gDialogLineNum, 20);

			if(saveScene_counter > 110 * FRAME_RATE_SCALER_INV && (sm64::player(0).controller().buttonPressed & sm64::hid::A_BUTTON || sm64::player(0).controller().buttonPressed & sm64::hid::START_BUTTON))
			{
				FreezeGame(0, 0);
				AudStartSound(SOUND_MENU_STAR_SOUND, gDefaultSoundArgs);
				gDialogBoxState		  = DIALOG_STATE_OPENING;
				gMenuMode		  = -1;
				num			  = gDialogLineNum;
				saveScene_counter	  = 0;
				gCourseCompleteCoins	  = 0;
				gCourseCompleteCoinsEqual = 0;
				getStar_flag		  = 0;

				return num;
			}
			break;
	}

	if(font_alpha < 250)
	{
		font_alpha += 25 / FRAME_RATE_SCALER_INV;
	}

	saveScene_counter++;

	return 0;
}

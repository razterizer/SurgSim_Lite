#include "TitleScreen.h"
#include "InstructionsScreen.h"
#include "CreateAnatomies.h"
#include "TubeTopology.h"
#include "Instruments.h"
#include "ToolMenus.h"
#include "Dissection.h"
#include "Liquids.h"
#include "HealthStates.h"
#include "Keyboard.h"
#include "Hud.h"
#include "Grasping.h"

#include <Termin8or/Screen.h>
#include <Core/Delay.h>
#include <Termin8or/GameEngine.h>
#include <Termin8or/ASCII_Fonts.h>

#include <iostream>

const int max_health = 30;
int health = max_health; // #HACKABLE
const int max_blood = 30;
int blood = 0;

// TODO:
// 1. --Render organ sprites. E.g. liver, gallbladder, cystic artery, cystic duct and hepatoduodenal ligament.
// 2. --Render and control instrument arms. Pivots are the lower corners of the screen.
// 3. --Simple instrument menu with only hook and scissors.
// 4. --Simple burning handler with coag and decay values. Map characters to values.
// 5. --Simple scissor cutting: Re-use decay field. For vessels any decay will cause sever.
// 6. --Render blood if cutting artery (scissor or hook) without proper coagulation. Simple animation.
// 7. --Achieved proper coagulation should be shown as white color. Add function to query sufficient coagulation.
// 8. --Register burn on gallbladder and liver. Decrease health.
// 9. --Stop bleeding using coagulation (normally too thick vessels/ducts for that but adds depth to the game).
// 10. --Register proper division of artery. If improperly divided, then win-screen only occurs after bleeding has been stopped.
// 11. --Increase score by 500 when properly dissected (hook/scissor) which makes the artery visible.
// 12. --Increase score by 1000 when properly divided vessel or by 300 when bleeding stopped after improper division.
// 13.x Increase score by 100 for every bleed stop by coag.
// 14. --Game Over when too much blood or no more health.
// 15. --Add error and warning messages.
// 16. --Make title screen.
// 17.x Gallbladder should move a little if pushed (or grasped) by instrument (tip still close to middle of gallbladder, then moving the tip).
// 18. --Blood pool (actually fluid pool now).
// 19.x More random decay patterns.
// 20. --Be able to burn on underlying tissue.
// 21. --Opaque background on tool icons.
// 22. --More easy to discern the artery tree from the liver.
// 23. --Be able to burn on the artery tree and the duct tree.
// 24.x Bug: Decaying some time on the gallbladder will eventually give you an error message for the liver.
// 25. --Bug: The game over screen shows up once then subsequent times it will not show.
// 26. --Bug: We don't want decay/coag to spread across multiple texture layers. If tcp is on a vessel, then it should not affect the liver.
// 27. --Let max coag spill over into decay so that charred tissue eventually will ablate away.
// 28. --Detect clips in neighbourhood of cut pixel. Duct topology?
// 29. --Clips should be more visible, especially on bright background.
// 30. --Implement grasping (for gallbladder only). It should be able to move the gallbladder one pixel N, S, W and E (or something like that).
// 31. --Implement suction. See if we can squeeze one more icon into the tool menus.
// 32. --Implement rinsing. If rinsing then liquid mixing must be "three-dimensional". Blue color for water.
// 33. --Make instruction screen.
// 34. --Rinsing should restore charred pixels (black) into fully coagulated ones (white).
// 35. --Replace old single droplet code for "tubes" with new particle/droplet system (DropletHandler).
// 36. --Scissors should not continuously cut. Use FlankDetector. Also, while artery/duct is visible, only one cut should suffice.
// 37.x Think about what is going to happen when bile is overflowing.
// 38. --Let bile flow diminish with time. Think Bernoulli's law.
// 39. --Rinsing droplets do not update when switching tool.
// 40. --Smoke when burning.
// 41. --Error and penalty when clipping the liver or gallbladder.
// 42. Bug: Bile fluid sometimes pauses and then suddenly continues again.
// 43. --Bug: Grasping is not properly reset when grasping tissue, then moving it and then switching instrument.
// 44. --Bug (rare): Gallbladder should not be able to move up when grasping.
// 45. Move implementations into source files.
// 46. --Bug: When grasping a texture layer and then moving it with the grasper, the grasper tcp should go back to the locked position.
// 47. --Bug: grasper tool head look weird in some positions when grasping and moving a texture layer.
// 48. --Bile leakage at root or blood leakage at root will cause game over screen, because there is no way to recover from that.
// 49. --Should be fatal when damaging coledocus, hepaticus, etc.
// 50. --Sparks when burning.

///////////////////////////////

class Game : public GameEngine<>
{
public:
  Game(int argc, char** argv, const GameEngineParams& params)
    : GameEngine(argv[0], params)
    , instr_data_left(InstrumentSide::Left, shaft_len, shaft_z_left, ang_left_rad, pix_ar)
    , instr_data_right(InstrumentSide::Right, shaft_len, shaft_z_right, ang_right_rad, pix_ar)
  {
    GameEngine::set_delay_us(200'000);
    if (argc >= 2)
      GameEngine::set_delay_us(atoi(argv[1]));

    if (argc >= 3)
    {
      //int alt_km = atoi(argv[2]);
      //y_pos = -alt_km*1e3/pix_to_m + ground_level + 13*pix_ar;
    }

    game_over_timer = 40;
  }

  virtual void generate_data() override
  {
    all_textures.resize(static_cast<size_t>(TextureType::NUM_ITEMS));
    create_liver(all_textures[static_cast<size_t>(TextureType::LIVER)]);
    create_gallbladder(all_textures[static_cast<size_t>(TextureType::GALLBLADDER)]);
    create_duct_tree(all_textures[static_cast<size_t>(TextureType::DUCT_TREE)]);
    create_artery_tree(all_textures[static_cast<size_t>(TextureType::ARTERY_TREE)]);
    create_hepatoduodenal_ligament(all_textures[static_cast<size_t>(TextureType::HD_LIG)]); // #HACK

    tube_topology::create_topologies();

    health_states.set_critical_blood_vol(12 * 80);
    
    std::string font_data_path = ASCII_Fonts::get_path_to_font_data();
    std::cout << font_data_path << std::endl;
    
    color_schemes.emplace_back();
    auto& cs = color_schemes.emplace_back();
    cs.internal.bg_color = Text::Color::Red;
    cs.side_h.bg_color = Text::Color::DarkRed;
    cs.dot_internal.bg_color = Text::Color::DarkGray;
    cs.dot_side_h.bg_color = Text::Color::Black;
    
    for (auto& cs : color_schemes)
      font_data.emplace_back(ASCII_Fonts::load_font_data(cs, font_data_path));
  }

private:
  virtual void update() override
  {
    Key curr_special_key = register_keypresses(kpd);

      trg_tool_left = anim_idx_curr_left == 1 && anim_idx_prev_left == 0;
      trg_tool_right = anim_idx_curr_right == 1 && anim_idx_prev_right == 0;
      anim_idx_prev_left = anim_idx_curr_left;
      anim_idx_prev_right = anim_idx_curr_right;

      RC tcp_rc_left, tcp_rc_right;

      draw_hud(sh, health, max_health, blood, max_blood, GameEngine::ref_score());

      draw_frame(sh, Text::Color::Black);

      if (health <= 0 || blood >= max_blood)
        GameEngine::set_state_game_over();
      else if (health_states.is_exercise_completed())
        GameEngine::set_state_you_won();

      handle_injuries(sh, msg_handler, health_states,
        health, blood, max_blood, GameEngine::ref_score(),
        time);
      if (GameEngine::ref_score() < 0)
        GameEngine::ref_score() = 0;

      health_states.check_visibility(all_textures, TextureType::HD_LIG, GameEngine::ref_score());
      health_states.check_correct_artery_division(all_textures, GameEngine::ref_score());
      health_states.check_correct_duct_division(all_textures, GameEngine::ref_score());
      health_states.check_liquid_pool_empty(GameEngine::ref_score());

      if (curr_special_key == Key::Menu)
        show_menus = !show_menus;
      if (show_menus)
      {
        draw_tool_menu(sh, InstrumentSide::Left, static_cast<int>(tool_type_left));
        draw_tool_menu(sh, InstrumentSide::Right, static_cast<int>(tool_type_right));

        auto step_tool = [](ToolType& tool_type, int step)
          {
            int idx = static_cast<int>(tool_type);
            idx += step;
            if (step > 0 && idx == icon_data::num_tool_types)
              idx = 0;
            else if (step < 0 && idx == -1)
              idx = icon_data::num_tool_types - 1;
            tool_type = static_cast<ToolType>(idx);
          };
        switch (curr_special_key)
        {
          case Key::LI_Up:   step_tool(tool_type_left, -1); break;
          case Key::LI_Down: step_tool(tool_type_left, +1); break;
          case Key::RI_Up:   step_tool(tool_type_right, -1); break;
          case Key::RI_Down: step_tool(tool_type_right, +1); break;
          default: break;
        }

        anim_idx_curr_left = 0;
        anim_idx_curr_right = 0;
      }
      else
      {
        // #HACK
        //if (anim_ctr == 1 || anim_ctr == 5)
        //  curr_key = Key::LI_Trigger;
        //else if (anim_ctr == 3)
        //  curr_key = Key::None;
        //curr_key = Key::Coag; // #HACK
        update_instruments(curr_special_key,
          tool_type_left, tool_type_right,
          anim_idx_curr_left, anim_idx_curr_right,
          instr_data_left, instr_data_right,
          time, pix_ar);
      }

      //tcp_rc_left = { 20, 43 }; // (cystic artery) #HACK
      //tcp_rc_left = { 12, 45 }; // (gallbladder) #HACK
      //instr_data_left.set(InstrumentSide::Left, tcp_rc_left); // #HACK
      grasp::handle_grasping(instr_data_left, instr_data_right,
        tool_type_left, tool_type_right,
        trg_tool_left, trg_tool_right,
        anim_idx_curr_left, anim_idx_curr_right,
        all_textures);
      tcp_rc_left = instr_data_left.get_tcp();
      tcp_rc_right = instr_data_right.get_tcp();

      draw_instruments(sh, InstrumentSide::Left,
        ang_left_rad,
        shaft_z_left,
        tool_type_left,
        anim_idx_curr_left,
        instr_data_left,
        pix_ar);
      draw_instruments(sh, InstrumentSide::Right,
        ang_right_rad,
        shaft_z_right,
        tool_type_right,
        anim_idx_curr_right,
        instr_data_right,
        pix_ar);

      generate_sparks(sh, curr_special_key,
        tcp_rc_left, tcp_rc_right,
        tool_type_left, tool_type_right,
        anim_ctr);
      generate_smoke(sh, curr_special_key,
        tcp_rc_left, tcp_rc_right,
        tool_type_left, tool_type_right,
        dt, time);

      std::vector<RC> fluid_sources;
      liquids::update_profuse_liquids(sh,
        curr_special_key,
        tcp_rc_left, tcp_rc_right,
        tool_type_left, tool_type_right,
        all_textures,
        liquid_volumes, liquid_flow, fluid_sources,
        time, dt);
      health_states.register_fluids(liquid_volumes, liquid_flow, fluid_sources);

      draw_anatomy(sh, all_textures[static_cast<size_t>(TextureType::HD_LIG)]);
      draw_anatomy(sh, all_textures[static_cast<size_t>(TextureType::ARTERY_TREE)]);
      draw_anatomy(sh, all_textures[static_cast<size_t>(TextureType::DUCT_TREE)]);
      draw_opaque_anatomy(sh, all_textures[static_cast<size_t>(TextureType::GALLBLADDER)]);
      draw_opaque_anatomy(sh, all_textures[static_cast<size_t>(TextureType::LIVER)]);
      draw_ground(sh);

      update_burn(dt, curr_special_key,
        tcp_rc_left, tcp_rc_right,
        tool_type_left, tool_type_right,
        trg_tool_left, trg_tool_right,
        all_textures,
        health_states);

      handle_clip_applying(curr_special_key,
        tcp_rc_left, tcp_rc_right,
        tool_type_left, tool_type_right,
        trg_tool_left, trg_tool_right,
        all_textures,
        health_states);
    ///

    //sh.replace_bg_color(Text::Color::Transparent, Text::Color::DarkBlue, { 1, 1, 77, h_offs });
  }
  
  virtual void draw_title() override
  {
    ::draw_title(sh, font_data);
  }
  
  virtual void draw_instructions() override
  {
    ::draw_instructions(sh, max_health);
  }

  //////////////////////////////////////////////////////////////////////////

  // Instruments
  float ang_left_rad = 0.7854 - 0.1f;
  float ang_right_rad = -0.7854f + 0.12f;
  const float shaft_len = 80.f;
  float shaft_z_left = 35.f;
  float shaft_z_right = 35.f;
  // Tool Menus
  ToolType tool_type_left = ToolType::Grasper; //ToolType::Hook; // #HACK
  ToolType tool_type_right = ToolType::Grasper;
  int anim_idx_curr_left = 0;
  int anim_idx_curr_right = 0;
  int anim_idx_prev_left = 0;
  int anim_idx_prev_right = 0;
  bool trg_tool_left = false;
  bool trg_tool_right = false;

  bool show_menus = false;

  InstrumentData instr_data_left, instr_data_right;

  std::vector<std::vector<AnatomyLineData>> all_textures;

  // States for messages and health penalty.
  HealthStates health_states;

  MessageHandler msg_handler;

  liquids::LiquidVolumes liquid_volumes;
  liquids::LiquidFlow liquid_flow;
  
  std::vector<ASCII_Fonts::ColorScheme> color_schemes;
  std::vector<ASCII_Fonts::FontDataColl> font_data;
};

//////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
  //initscr();
  //noecho();
  //cbreak();
  //keypad(stdscr, true);

  if (argc >= 2 && !strcmp(argv[1], "--help"))
  {
    std::cout << "surgsim_lite (\"--help\" | [<frame-delay-us> [<nåt-här>]])" << std::endl;
    return EXIT_SUCCESS;
  }
  
  GameEngineParams params;
  params.screen_bg_color_default = Text::Color::DarkMagenta;
  params.screen_bg_color_title = Text::Color::LightGray;
  params.screen_bg_color_instructions = Text::Color::Black;

  //nodelay(stdscr, TRUE);
  
  Game game(argc, argv, params);
  game.init();
  game.generate_data();
  game.run();

  return EXIT_SUCCESS;
}

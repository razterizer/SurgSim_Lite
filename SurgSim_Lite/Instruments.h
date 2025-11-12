#pragma once
#include "InstrumentData.h"
#include <Termin8or/geom/RC.h>
#include <Core/MathUtils.h>
#include <Termin8or/drawing/Drawing.h>
#include "Enums.h"
#include "icon_data.h"


float close_time_stamp_left = 0.f;
float close_time_stamp_right = 0.f;
bool curr_trg_left = false;
bool last_trg_left = false;
bool curr_trg_right = false;
bool last_trg_right = false;

void update_instruments(Key key,
                        ToolType tool_type_left, ToolType tool_type_right,
                        int& anim_idx_left, int& anim_idx_right,
                        InstrumentData& instr_data_left, InstrumentData& instr_data_right,
                        float curr_time, const float pix_ar)
{
  const float ang_step_rad = math::deg2rad(1.f);
  const float z_step = 1.f;
  const float min_close_time = 0.2f;
  curr_trg_left = false;
  curr_trg_right = false;
  switch (key)
  {
    case Key::LI_Left:
      instr_data_left.step_angle(+ang_step_rad);
      break;
    case Key::LI_Right:
      instr_data_left.step_angle(-ang_step_rad);
      break;
    case Key::LI_Up:
      instr_data_left.step_insertion(+z_step);
      break;
    case Key::LI_Down:
      instr_data_left.step_insertion(-z_step);
      break;
    case Key::RI_Left:
      instr_data_right.step_angle(+ang_step_rad);
      break;
    case Key::RI_Right:
      instr_data_right.step_angle(-ang_step_rad);
      break;
    case Key::RI_Up:
      instr_data_right.step_insertion(+z_step);
      break;
    case Key::RI_Down:
      instr_data_right.step_insertion(-z_step);
      break;
    case Key::LI_Trigger:
      curr_trg_left = true;
      if (last_trg_left != curr_trg_left && tool_type_left != ToolType::Scissors)
        anim_idx_left = 1 - anim_idx_left;
      else
        anim_idx_left = 1;
      close_time_stamp_left = curr_time;
      break;
    case Key::RI_Trigger:
      curr_trg_right = true;
      if (last_trg_right != curr_trg_right && tool_type_right != ToolType::Scissors)
        anim_idx_right = 1 - anim_idx_right;
      else
        anim_idx_right = 1;
      close_time_stamp_right = curr_time;
    default:
      if (tool_type_left == ToolType::Scissors && curr_time - close_time_stamp_left > min_close_time)
        anim_idx_left = 0;
      if (tool_type_right == ToolType::Scissors && curr_time - close_time_stamp_right > min_close_time)
        anim_idx_right = 0;
      break;
  }
  last_trg_left = curr_trg_left;
  last_trg_right = curr_trg_right;
  
  instr_data_left.update();
  instr_data_right.update();
}

//        //=
//       ##
//     ##
//   ##
// ##
//         ?
//       ##
//     ##
//   ##
// ##
//         /_
//       ##
//     ##
//   ##
// ##
//        )(
//       ##
//     ##
//   ##
// ##


template<int NR, int NC>
void draw_instruments(ScreenHandler<NR, NC>& sh, InstrumentSide side,
                      float ang_rad, float shaft_z,
                      ToolType tool_type, int anim_idx,
                      const InstrumentData& instr_data,
                      const float pix_ar)
{
  const auto fg_color_shaft = Color::Black;
  const auto bg_color_shaft = Color::DarkGray;
  
  const auto& texture = icon_data::side_anim_tool_textures[static_cast<int>(side)][anim_idx][static_cast<int>(tool_type)];
  auto tcp = instr_data.get_tcp();
  if (side == InstrumentSide::Right)
    tcp.c -= static_cast<int>(texture.str.size()) - 1;
  for (const auto& pixel : texture.get_pixels())
    sh.write_buffer(pixel.str, tcp.r, tcp.c++, pixel.fg_color, pixel.bg_color);
  
  t8x::plot_line(sh,
                       instr_data.get_pivot_y(), instr_data.get_pivot_x(),
                       instr_data.get_end_y(), instr_data.get_end_x(),
                       "#", fg_color_shaft, bg_color_shaft);
}


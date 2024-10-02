#pragma once
#include "Enums.h"
#include "icon_data.h"

template<int NR, int NC>
void draw_tool_menu(SpriteHandler<NR, NC>& sh, InstrumentSide side, int sel_idx)
{
  auto draw_icon = [&](ToolType tool_type, int r, int c, bool selected)
  {
    auto tool_idx = static_cast<int>(tool_type);
    const auto& icon = icon_data::tool_icons[tool_idx];
    int rr = r;
    int cc = c;
    for (const auto& line : icon)
    {
      cc = c;
      for (const auto& pixel : line.get_pixels())
      {
        sh.write_buffer(pixel.str, rr, cc, pixel.fg_color, pixel.bg_color);
        cc++;
      }
      if (selected)
        sh.write_buffer(str::rep_char(' ', 9), rr, c, Color::Default, Color::Blue);
      else
        sh.write_buffer(str::rep_char(' ', 9), rr, c, Color::Default, Color::Magenta);
      rr++;
    }
  };
  
  auto draw_label = [&](ToolType tool_type, InstrumentSide side, int r, int c, bool selected)
  {
    auto tool_idx = static_cast<int>(tool_type);
    const auto& label = icon_data::tool_labels[tool_idx];
    const int rr = r;
    int cc = c;
    if (side == InstrumentSide::Right)
      cc -= static_cast<int>(label.str.size());
    for (const auto& pixel : label.get_pixels())
    {
      auto bg_color = selected ? Color::Cyan : Color::DarkCyan; //pixel.bg_color;
      sh.write_buffer(pixel.str, rr, cc, pixel.fg_color, bg_color);
      cc++;
    }
  };
  
  auto draw_icon_border = [&sh](int r, int c)
  {
    const auto blk = Color::Black;
    sh.write_buffer("+--------+", r+0, c, blk);
    sh.write_buffer("|        |", r+1, c, blk);
    sh.write_buffer("|        |", r+2, c, blk);
    sh.write_buffer("|        |", r+3, c, blk);
    sh.write_buffer("|        |", r+4, c, blk);
    //sh.write_buffer("|        |", r+5, c, blk);
    sh.write_buffer("+--------+", r+5, c, blk);
  };
  
  int r = 2;//5;
  int c = 0;
  switch (side)
  {
    case InstrumentSide::Left:
      c = 1;
      break;
    case InstrumentSide::Right:
      c = (80 - 1) - 10;
      break;
    case InstrumentSide::NUM_ITEMS:
      break;
  }
  for (int tool_idx = 0; tool_idx < icon_data::num_tool_types; ++tool_idx)
  {
    auto tool_type = static_cast<ToolType>(tool_idx);
    draw_icon_border(r-1, c);
    draw_icon(tool_type, r, c+1, tool_idx == sel_idx);
    draw_label(tool_type, side, r+1, side == InstrumentSide::Left ? c+10 : c, tool_idx == sel_idx);
    r += icon_data::num_tool_icon_lines + 1;
  }
}


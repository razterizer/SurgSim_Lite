#pragma once
#include <Termin8or/LineData.h>

namespace icon_data
{
  const auto blk = Text::Color::Black;
  const auto dgr = Text::Color::DarkGray;
  const auto lgr = Text::Color::LightGray;
  const auto wht = Text::Color::White;
  const auto cyn = Text::Color::Cyan;
  const auto tr2 = Text::Color::Transparent2;
  const auto num_tool_types = static_cast<int>(ToolType::NUM_ITEMS);
  const auto num_tool_icon_lines = 4;

  const std::array<std::array<std::array<LineData, num_tool_types>, 2>, 2> side_anim_tool_textures
  {
    // Left
    std::array<std::array<LineData, num_tool_types>, 2>
    {
      // Open
      std::array<LineData, num_tool_types>
      {
        LineData
        { "||=", { blk, blk, blk }, { lgr, lgr, lgr } },
        { "?", { blk }, { tr2 } },
        { "|_", { blk, blk }, { tr2, tr2 } },
        { ")(", { wht, wht }, { dgr, dgr } },
        { "\\\\", { lgr, wht }, { dgr, dgr } },
      },
      // Close
      {
        LineData
        { "///", { blk, blk, blk }, { lgr, dgr, lgr } },
        { "?", { blk }, { tr2 } },
        { "//", { blk, blk }, { tr2, tr2 } },
        { "//", { wht, wht }, { dgr, dgr } },
        { "\\\\", { lgr, wht }, { dgr, dgr } },
      }
    },
    // Right
    {
      // Open
      std::array<LineData, num_tool_types>
      {
        LineData
        { "=||", { blk, blk, blk }, { lgr, lgr, lgr } },
        { "?", { blk }, { tr2 } },
        { "_|", { blk, blk }, { tr2, tr2 } },
        { ")(", { wht, wht }, { dgr, dgr } },
        { "//", { lgr, wht }, { dgr, dgr } },
      },
      // Close
      {
        LineData
        { "\\\\\\", { blk, blk, blk }, { lgr, dgr, lgr } },
        { "?", { blk }, { tr2 } },
        { "\\\\", { blk, blk }, { tr2, tr2 } },
        { "\\\\", { wht, wht }, { dgr, dgr } },
        { "//", { lgr, wht }, { dgr, dgr } },
      }
    },
  };

  std::array<std::array<LineData, num_tool_icon_lines>, num_tool_types> tool_icons
  {
    // Grasper
    std::array<LineData, 4>
    {
      LineData
      { "/ | / /" , { blk, blk, blk, blk, blk, blk, blk }, { dgr, lgr, wht, tr2, dgr, lgr, dgr } },
      { "| |/ / " , { blk, blk, blk, blk, blk, blk, blk }, { dgr, lgr, wht, dgr, lgr, dgr, tr2 } },
      { " \\v/   ", { blk, blk, blk, blk, blk, blk, blk }, { tr2, dgr, wht, dgr, tr2, tr2, tr2 } },
      { " |#|   " , { blk, blk, blk, blk, blk, blk, blk }, { tr2, dgr, lgr, blk, tr2, tr2, tr2 } },
      //{ "GRASPER" , { blk, blk, blk, blk, blk, blk, blk }, { cyn, cyn, cyn, cyn, cyn, cyn, cyn } },
    },
    // Hook
    {
      LineData
      { "   +-" , { blk, blk, blk, blk, blk }, { tr2, tr2, tr2, lgr, lgr } },
      { "   | " , { blk, blk, blk, blk, blk }, { tr2, tr2, tr2, lgr, tr2 } },
      { "  /#\\", { blk, blk, blk, blk, blk }, { tr2, tr2, dgr, wht, blk } },
      { "  |#|" , { blk, blk, blk, blk, blk }, { tr2, tr2, dgr, lgr, blk } },
      //{ "HOOK" ,   { blk, blk, blk, blk }, { cyn, cyn, cyn, cyn } },
    },
    // Scissors
    {
      LineData
      { " /| |\\",  { blk, blk, blk, blk, blk, blk }, { tr2, dgr, wht, tr2, dgr, lgr } },
      { " || ||" ,  { blk, blk, blk, blk, blk, blk }, { tr2, dgr, wht, tr2, dgr, lgr } },
      { "  \\v/ ",  { blk, blk, blk, blk, blk, blk }, { tr2, tr2, dgr, wht, dgr, tr2 } },
      { "  |#| "  , { blk, blk, blk, blk, blk, blk }, { tr2, tr2, dgr, lgr, blk, tr2 } },
      //{ "SCISSORS" ,     { blk, blk, blk, blk, blk, blk, blk, blk }, { cyn, cyn, cyn, cyn, cyn, cyn, cyn, cyn } },
    },
    // ClipApplier
    {
      LineData
      { " /) (\\", { blk, blk, blk, blk, blk, blk }, { tr2, dgr, wht, tr2, wht, lgr } },
      { " |\\./|", { blk, blk, blk, blk, blk, blk }, { tr2, dgr, lgr, tr2, dgr, lgr } },
      { " \\   /", { blk, blk, blk, blk, blk, blk }, { tr2, dgr, dgr, lgr, dgr, lgr } },
      { "  |#|"  , { blk, blk, blk, blk, blk }, { tr2, tr2, dgr, lgr, blk} },
      //{ "CLIP APP" ,   { blk, blk, blk, blk, blk, blk, blk, blk, blk }, { cyn, cyn, cyn, cyn, cyn, cyn, cyn, cyn, tr2 } },
    },
    // Suction/Rinsing
    {
      LineData
      { "    /)", { blk, blk, blk, blk, blk, blk }, { tr2, tr2, tr2, tr2, dgr, lgr }},
      { "   /./", { blk, blk, blk, blk, blk, blk }, { tr2, tr2, tr2, dgr, lgr, wht }},
      { "  /./ ", { blk, blk, blk, blk, blk, blk }, { tr2, tr2, dgr, lgr, wht, tr2 }},
      { " /./  ", { blk, blk, blk, blk, blk, blk }, { tr2, dgr, lgr, wht, tr2, tr2 }},
    }
  };

  std::array<LineData, num_tool_types> tool_labels
  {
    {
      LineData
      { "GRASPER" , { blk, blk, blk, blk, blk, blk, blk }, { cyn, cyn, cyn, cyn, cyn, cyn, cyn } },
      { "HOOK" , { blk, blk, blk, blk }, { cyn, cyn, cyn, cyn } },
      { "SCISSORS" , { blk, blk, blk, blk, blk, blk, blk, blk }, { cyn, cyn, cyn, cyn, cyn, cyn, cyn, cyn } },
      { "CLIP APPLIER" , { blk, blk, blk, blk, blk, blk, blk, blk, blk, blk, blk, blk }, { cyn, cyn, cyn, cyn, cyn, cyn, cyn, cyn, cyn, cyn, cyn, cyn } },
      { "SUCT./RINS.", { blk, blk, blk, blk, blk, blk, blk, blk, blk, blk, blk }, { cyn, cyn, cyn, cyn, cyn, cyn, cyn, cyn, cyn, cyn, cyn } },
    }
  };

}


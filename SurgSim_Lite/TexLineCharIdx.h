#pragma once

struct TexLineCharIdx
{
  TexLineCharIdx() = default;
  TexLineCharIdx(int t_idx, int ln_idx, int ch_idx) : tex_idx(t_idx), line_idx(ln_idx), char_idx(ch_idx) { }
  int tex_idx = -1; // Texture index.
  int line_idx = -1;
  int char_idx = -1;
};

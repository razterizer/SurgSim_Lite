#pragma once
#include <Termin8or/Gradient.h>
#include <Core/Physics.h>
#include <Termin8or/Pixel.h>
#include <Termin8or/RC.h>
#include <Core/StringHelper.h>
#include <Termin8or/ScreenHandler.h>
#include "Enums.h"
#include "TexLineCharIdx.h"


struct AnatomyLineData
{
  std::vector<float> coag;
  std::vector<float> decay;
  //std::vector<float> temperature_K;
  std::vector<int> clip;
  std::string str;
  std::vector<Color> fg_colors;
  Color fg_cut_color;
  bool tube = false;
  std::vector<Gradient<Color>> bg_color_gradients;
  size_t N = 0;
  int r = -1;
  int r_orig = -1;
  std::vector<int> c;
  std::vector<int> c_orig;
  int line_idx = -1;

  AnatomyLineData(size_t num)
    : N(num)
  {
    str = str::rep_char(' ', static_cast<int>(N));
    fg_colors.resize(N);
    bg_color_gradients.resize(N);
    //temperature_K.resize(N);
    //std::fill(temperature_K.begin(), temperature_K.end(), deg_celcius_to_kelvin(37.f));
    coag.resize(N);
    decay.resize(N);
    clip.resize(N);
    c.resize(N);
    c_orig.resize(N);
    std::fill(c.begin(), c.end(), -1);
    std::fill(c_orig.begin(), c_orig.end(), -1);
  }
  AnatomyLineData(const std::string& s, const std::vector<Color>& fg, Color fg_cut, const std::vector<Gradient<Color>>& bg_col_grad)
    : str(s), fg_colors(fg), fg_cut_color(fg_cut), bg_color_gradients(bg_col_grad)
  {
    size_t N = std::min(str.size(), std::min(fg_colors.size(), bg_color_gradients.size()));
    //temperature_K.resize(N);
    //std::fill(temperature_K.begin(), temperature_K.end(), deg_celcius_to_kelvin(37.f));
    coag.resize(N);
    decay.resize(N);
    clip.resize(N);
    c.resize(N);
    c_orig.resize(N);
    std::fill(c.begin(), c.end(), -1);
    std::fill(c_orig.begin(), c_orig.end(), -1);
  }
  
  std::vector<Pixel> get_pixels() const
  {
    std::vector<Pixel> pixels;
    for (size_t i = 0; i < N; ++i)
    {
      float coag_i = coag[i];
      float decay_i = decay[i];
      auto fg_color = fg_cut_color;
      auto bg_color = bg_color_gradients[i](coag_i);
      char ch = str[i];
      bool active = true;
      if (!tube)
      {
        if (0.5f <= decay_i && decay_i < 0.6f)
          ch = '(';
        else if (0.6f <= decay_i && decay_i < 0.7f)
          ch = ')';
        else if (0.7f <= decay_i && decay_i < 0.8f)
          ch = '}';
        else if (0.8f <= decay_i && decay_i < 0.9f)
          ch = '{';
        else if (0.9f <= decay_i && decay_i < 0.95f)
          ch = ':';
        else if (0.95f <= decay_i && decay_i <= 1.f)
          active = false;
        else
          fg_color = fg_colors[i];
      }
      else
      {
        fg_color = fg_colors[i];
        if (decay_i > 0.7f)
          active = false;
      }
      if (clip[i] == 1 && active)
      {
        fg_color = Color::LightGray;
        if (bg_color == Color::Magenta || bg_color == Color::LightGray || bg_color == Color::White || bg_color == Color::Yellow)
          fg_color = Color::DarkGray;
        ch = '/';
      }
      pixels.emplace_back(ch, fg_color, bg_color, r, c[i], line_idx, static_cast<int>(i), active);
    }
    return pixels;
  }
};

void create_texture_row_uniform(std::vector<AnatomyLineData>& texture,
                                char ch_left, char ch_inner, char ch_right,
                                int r, int c_from, int c_to,
                                Color fg_color, Color fg_cut_color, const Gradient<Color>& bg_color_gradient)
{
  AnatomyLineData line(c_to - c_from + 1);
  line.r = r;
  line.r_orig = r;
  line.fg_cut_color = fg_cut_color;
  int i = 0;
  for (int c = c_from; c <= c_to; ++c)
  {
    if (c == c_from)
      line.str[i] = ch_left;
    else if (c == c_to)
      line.str[i] = ch_right;
    else
      line.str[i] = ch_inner;
    line.fg_colors[i] = fg_color;
    line.bg_color_gradients[i] = bg_color_gradient;
    line.c[i] = c;
    line.c_orig[i] = c;
    i++;
  }
  line.line_idx = static_cast<int>(texture.size());
  texture.emplace_back(line);
}

void create_texture_row_tube(std::vector<AnatomyLineData>& texture,
                             const std::string& str,
                             int r, int c,
                             Color fg_color, const Gradient<Color>& bg_color_gradient)
{
  int N = static_cast<int>(str.size());
  AnatomyLineData line(N);
  line.r = r;
  line.tube = true;
  for (int i = 0; i < N; ++i)
  {
    line.fg_colors[i] = fg_color;
    line.bg_color_gradients[i] = bg_color_gradient;
    line.c[i] = c + i;
  }
  line.line_idx = static_cast<int>(texture.size());
  texture.emplace_back(line);
}

template<int NR, int NC>
void draw_opaque_anatomy(ScreenHandler<NR, NC>& sh, const std::vector<AnatomyLineData>& texture)
{
  for (const auto& line : texture)
    for (const auto& pixel : line.get_pixels())
      sh.write_buffer(pixel.str, pixel.r, pixel.c, pixel.fg_color, pixel.bg_color);
}

template<int NR, int NC>
void draw_anatomy(ScreenHandler<NR, NC>& sh, const std::vector<AnatomyLineData>& texture)
{
  for (const auto& line : texture)
    for (const auto& pixel : line.get_pixels())
      if (pixel.active)
        sh.write_buffer(pixel.str, pixel.r, pixel.c, pixel.fg_color, pixel.bg_color);
}

auto find_pixel = [](const std::vector<std::vector<AnatomyLineData>>& textures, TextureType tex_type, const RC& rc) -> TexLineCharIdx
{
  int tex_idx = static_cast<int>(tex_type);
  const auto& tex = textures[tex_idx];
  for (const auto& line : tex)
    if (line.r == rc.r)
      for (const auto& pixel : line.get_pixels())
        if (pixel.c == rc.c)
          return { tex_idx, pixel.line_idx, pixel.char_idx };
  return {};
};

auto find_top_pixel = [](const std::vector<std::vector<AnatomyLineData>>& textures, const RC& rc, bool only_active, bool orig_space) -> TexLineCharIdx
{
  int N_tex = static_cast<int>(TextureType::NUM_ITEMS);
  for (int tex_idx = 0; tex_idx < N_tex; ++tex_idx)
  {
    const auto& tex = textures[tex_idx];
    for (const auto& line : tex)
      if ((orig_space ? line.r_orig : line.r) == rc.r)
        for (const auto& pixel : line.get_pixels())
          if ((orig_space ? pixel.c_orig : pixel.c) == rc.c && (!only_active || pixel.active))
            return { tex_idx, pixel.line_idx, pixel.char_idx };
  }
  return {};
};

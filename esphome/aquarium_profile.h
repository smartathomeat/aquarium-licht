#pragma once

// Reine Profilberechnung; derselbe Quelltext wird in ESPHome eingebettet.
struct AquariumProfile { int channel[3]; };
inline AquariumProfile aquarium_profile(double minute, int date_seed, double intensity,
                                        bool moon, bool clouds) {
  AquariumProfile result = {{0, 0, 0}};
  if (!(minute >= 0.0 && minute < 1440.0)) return result;
  if (!(intensity >= 0.0)) intensity = 0.0;
  if (intensity > 100.0) intensity = 100.0;

  // Einmalige Stuetzpunkte: Minuten seit Mitternacht, Weiss, Rot, Blau.
  const int curve[][4] = {
    {0,0,0,0}, {540,0,0,0}, {555,5,12,2}, {570,20,25,7},
    {585,100,35,20}, {975,100,35,20}, {990,25,30,15},
    {1005,5,12,5}, {1020,0,0,0}, {1440,0,0,0}
  };
  double shade = 1.0;
  if (clouds && minute >= 585.0 && minute < 975.0) {
    const int centers[] = {660 + date_seed % 21,
                           780 + (date_seed * 7) % 25,
                           910 + (date_seed * 3) % 21};
    const int half_widths[] = {6, 8, 7};
    for (int c = 0; c < 3; ++c) {
      double distance = minute - centers[c];
      if (distance < 0.0) distance = -distance;
      distance /= half_widths[c];
      if (distance < 1.0)
        shade = 1.0 - 0.25 * (1.0 - distance * distance * (3.0 - 2.0 * distance));
    }
  }

  for (int c = 0; c < 3; ++c) {
    if (moon && c == 2 && minute >= 1020.0 && minute < 1140.0) {
      result.channel[c] = 2;  // Mondlicht wird nicht mit der Intensitaet skaliert.
      continue;
    }
    for (unsigned i = 0; i + 1 < sizeof(curve) / sizeof(curve[0]); ++i) {
      if (minute >= curve[i][0] && minute < curve[i + 1][0]) {
        const double fraction = (minute - curve[i][0]) /
                                (curve[i + 1][0] - curve[i][0]);
        const double base = curve[i][c + 1] + fraction *
                            (curve[i + 1][c + 1] - curve[i][c + 1]);
        const int rounded = static_cast<int>(base * intensity / 100.0 * shade + 0.5);
        result.channel[c] = rounded < 2 ? 0 : (rounded > 100 ? 100 : rounded);
        break;
      }
    }
  }
  return result;
}

#include <cmath>
#include <iostream>
#include <stdexcept>
#include "aquarium_profile.h"

static void expect(double minute, double intensity, bool moon, bool cloud,
                   int w, int r, int b, int seed = 20261118) {
  const auto actual = aquarium_profile(minute, seed, intensity, moon, cloud);
  const int wanted[] = {w, r, b};
  for (int i = 0; i < 3; ++i) {
    if (actual.channel[i] != wanted[i]) {
      std::cerr << "Minute " << minute << ", Kanal " << i << ": "
                << actual.channel[i] << " statt " << wanted[i] << '\n';
      throw std::runtime_error("Abweichender Tagesverlauf");
    }
  }
}

int main() {
  expect(720, 100, false, false, 100, 35, 20);
  expect(0, 100, false, false, 0, 0, 0);
  expect(540, 100, false, false, 0, 0, 0);
  expect(555, 100, false, false, 5, 12, 2);
  expect(560, 100, false, false, 10, 16, 4);
  expect(560, 60, false, false, 6, 10, 2);
  expect(570, 100, false, false, 20, 25, 7);
  expect(585, 100, false, false, 100, 35, 20);
  expect(975, 100, false, false, 100, 35, 20);
  expect(980, 100, false, false, 75, 33, 18);
  expect(990, 100, false, false, 25, 30, 15);
  expect(1005, 100, false, false, 5, 12, 5);
  expect(1020, 100, false, false, 0, 0, 0);
  expect(1020, 100, true, false, 0, 0, 2);
  expect(1139.9, 0, true, true, 0, 0, 2);
  expect(1140, 100, true, false, 0, 0, 0);
  expect(720, 60, false, false, 60, 21, 12);
  expect(720, 0, false, true, 0, 0, 0);
  expect(720, -5, false, false, 0, 0, 0);
  expect(720, 130, false, false, 100, 35, 20);
  const int seed = 20261118;
  expect(660 + seed % 21, 100, false, true, 75, 26, 15);
  expect(-1, 100, true, true, 0, 0, 0);
  expect(1440, 100, true, true, 0, 0, 0);

  // Jede Minute und alle Schalterkombinationen: Nacht, Grenzen, Flackerschwelle.
  int cases = 0;
  for (int day : {20261118, 20261231, 20270328}) {
    for (int minute = 0; minute < 1440; ++minute) {
      for (double intensity : {0.0, 60.0, 100.0}) {
        for (bool moon : {false, true}) {
          for (bool clouds : {false, true}) {
            const auto p = aquarium_profile(minute, day, intensity, moon, clouds);
            for (int c = 0; c < 3; ++c) {
              if (p.channel[c] < 0 || p.channel[c] > 100 || p.channel[c] == 1)
                throw std::runtime_error("Grenze oder Flackerschwelle verletzt");
              const bool moon_window = moon && c == 2 && minute >= 1020 && minute < 1140;
              if ((minute < 540 || minute >= 1020) && !moon_window && p.channel[c] != 0)
                throw std::runtime_error("Licht ausserhalb des Tagesfensters");
              if (moon_window && p.channel[c] != 2)
                throw std::runtime_error("Mondlicht wurde skaliert");
            }
            ++cases;
          }
        }
      }
    }
  }
  std::cout << "Profiltests bestanden: " << cases << " Minuten-/Einstellungskombinationen plus Stuetzpunkte.\n";
}

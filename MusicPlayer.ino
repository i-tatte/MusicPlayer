#include <M5Stack.h>
#include "Note.h"
#include "Queue.h"
#include "Tone.h"
#include "utility/MPU9250.h"

#define NO -1

#define C1 33
#define CS1 35
#define D1 37
#define DS1 39
#define E1 41
#define F1 44
#define FS1 46
#define G1 49
#define GS1 52
#define A1 55
#define AS1 58
#define B1 62

#define C2 65
#define CS2 69
#define D2 73
#define DS2 78
#define E2 82
#define F2 87
#define FS2 92
#define G2 98
#define GS2 104
#define A2 110
#define AS2 117
#define B2 123

#define C3 131
#define CS3 139
#define D3 147
#define DS3 156
#define E3 165
#define F3 175
#define FS3 185
#define G3 196
#define GS3 208
#define A3 220
#define AS3 233
#define B3 247

#define C4 262
#define CS4 277
#define D4 294
#define DS4 311
#define E4 330
#define F4 349
#define FS4 370
#define G4 392
#define GS4 415
#define A4 440
#define AS4 466
#define B4 494

#define C5 523
#define CS5 554
#define D5 587
#define DS5 622
#define E5 659
#define F5 698
#define FS5 740
#define G5 784
#define GS5 831
#define A5 880
#define AS5 932
#define B5 988

#define C6 1047
#define CS6 1109
#define D6 1175
#define DS6 1245
#define E6 1319
#define F6 1397
#define FS6 1480
#define G6 1568
#define GS6 1661
#define A6 1760
#define AS6 1865
#define B6 1976

#define C7 2093
#define CS7 2217
#define D7 2349
#define DS7 2489
#define E7 2637
#define F7 2794
#define FS7 2960
#define G7 3136
#define GS7 3322
#define A7 3520
#define AS7 3729
#define B7 3951

#define speakerPin 25

#define musicSize 1018

MPU9250 imu;

bool countFlag = false;
long offTime = 0;
int beatTime = 500000;
int bpm = 120;
Queue rhythms = Queue();
Tone tones = Tone();

bool nowPlaying = false;
bool started = false;
int displayMode = 0;
long pausedTime = 0;
long startTime = 0;

float timeList[] = {7.0, 7.0, 7.272917, 7.5, 7.5, 7.75, 7.7916665, 8.0, 8.0, 8.008333, 8.095833, 8.31875, 8.5, 8.5, 8.602083, 8.75, 8.770834, 8.966667, 9.0, 9.0, 9.022917, 9.085417, 9.14375, 9.25, 9.5, 9.5, 9.520834, 9.739584, 9.75, 10.0, 10.0, 10.00625, 10.020834, 10.25, 10.258333, 10.5, 10.5, 10.527083, 10.564583, 10.75, 10.802084, 10.989584, 10.991667, 11.0, 11.0, 11.085417, 11.320833, 11.5, 11.5, 11.741667, 11.75, 12.0, 12.0, 12.047916, 12.13125, 12.210417, 12.5, 12.5, 12.533334, 12.75, 12.758333, 13.0, 13.0, 13.002084, 13.047916, 13.072917, 13.25, 13.29375, 13.5, 13.5, 13.527083, 13.747916, 13.75, 14.0, 14.0, 14.04375, 14.120833, 14.25, 14.2875, 14.5, 14.5, 14.5625, 14.5625, 14.59375, 14.75, 14.766666, 15.0, 15.0, 15.035417, 15.085417, 15.375, 15.5, 15.5, 15.75, 15.785417, 16.0, 16.0, 16.016666, 16.091667, 16.4375, 16.5, 16.5, 16.539583, 16.75, 16.7625, 17.0, 17.0, 17.004166, 17.010418, 17.020832, 17.25, 17.2875, 17.5, 17.5, 17.516666, 17.75, 17.777084, 18.0, 18.0, 18.004166, 18.066666, 18.25, 18.2625, 18.5, 18.5, 18.529167, 18.535416, 18.75, 18.79375, 19.0, 19.0, 19.035418, 19.05, 19.070833, 19.416666, 19.5, 19.5, 19.75, 19.754168, 20.0, 20.0, 20.05, 20.14375, 20.410418, 20.5, 20.5, 20.54375, 20.75, 20.777082, 20.989584, 20.991667, 21.0, 21.0, 21.020834, 21.25, 21.329166, 21.5, 21.5, 21.527082, 21.75, 21.766666, 22.0, 22.0, 22.045834, 22.158333, 22.25, 22.314583, 22.5, 22.5, 22.510416, 22.547916, 22.75, 22.777082, 22.9, 22.960417, 23.0, 23.0, 23.0, 23.05625, 23.279167, 23.35625, 23.5, 23.5, 23.5, 23.745832, 23.75, 23.75, 23.783333, 24.0, 24.0, 24.0, 24.020834, 24.035418, 24.095833, 24.275, 24.35625, 24.5, 24.5, 24.602083, 24.75, 24.785418, 24.9875, 25.0, 25.0, 25.022917, 25.085417, 25.25, 25.26875, 25.5, 25.5, 25.53125, 25.75, 25.783333, 26.0, 26.0, 26.00625, 26.014584, 26.25, 26.297916, 26.5, 26.5, 26.5125, 26.564583, 26.75, 26.779167, 26.9875, 26.989584, 26.991667, 27.0, 27.0, 27.391666, 27.5, 27.5, 27.5, 27.735416, 27.75, 27.75, 27.758333, 27.99375, 28.0, 28.0, 28.0, 28.033333, 28.13125, 28.252083, 28.489584, 28.5, 28.5, 28.533333, 28.75, 28.785418, 28.785418, 28.997917, 29.0, 29.0, 29.072916, 29.25, 29.28125, 29.5, 29.5, 29.545834, 29.75, 29.772917, 30.0, 30.0, 30.039583, 30.120832, 30.245832, 30.25, 30.5, 30.5, 30.53125, 30.59375, 30.75, 30.777082, 30.995832, 31.0, 31.0, 31.052084, 31.085417, 31.45625, 31.5, 31.5, 31.747917, 31.75, 32.0, 32.0, 32.0, 32.058334, 32.091667, 32.35, 32.427086, 32.5, 32.5, 32.53958, 32.735416, 32.75, 33.0, 33.0, 33.004166, 33.0125, 33.020836, 33.25, 33.30417, 33.5, 33.5, 33.533333, 33.75, 33.760414, 34.0, 34.0, 34.066666, 34.07083, 34.25, 34.29375, 34.46875, 34.5, 34.5, 34.529167, 34.75, 34.77292, 35.0, 35.0, 35.0, 35.020832, 35.05, 35.070835, 35.304165, 35.360416, 35.5, 35.5, 35.5, 35.745834, 35.75, 35.75, 35.76875, 36.0, 36.0, 36.0, 36.008335, 36.010418, 36.14375, 36.35, 36.4375, 36.5, 36.5, 36.54375, 36.75, 36.772915, 36.989582, 36.991665, 37.0, 37.0, 37.039585, 37.25, 37.26875, 37.5, 37.5, 37.529167, 37.75, 37.808334, 38.0, 38.0, 38.052082, 38.158333, 38.25, 38.308334, 38.5, 38.5, 38.547916, 38.5625, 38.75, 38.83125, 38.9, 38.960415, 39.0, 39.0, 39.0, 39.0875, 39.5, 39.5, 39.75, 39.789585, 40.0, 40.0, 40.058334, 40.095833, 40.41875, 40.5, 40.602085, 40.739582, 40.75, 40.908333, 40.908333, 41.0, 41.0, 41.0, 41.0, 41.039585, 41.085415, 41.25, 41.266666, 41.491665, 41.5, 41.5, 41.75, 41.7875, 42.0, 42.0, 42.00625, 42.058334, 42.25, 42.275, 42.495834, 42.5, 42.564583, 42.75, 42.770832, 42.991665, 43.0, 43.0, 43.0, 43.0, 43.027084, 43.04375, 43.077084, 43.44375, 43.5, 43.5, 43.75, 43.7625, 44.0, 44.0, 44.029167, 44.13125, 44.333332, 44.5, 44.533333, 44.75, 44.783333, 44.997917, 45.0, 45.0, 45.0, 45.0, 45.0125, 45.027084, 45.072918, 45.25, 45.289585, 45.5, 45.5, 45.547916, 45.75, 45.76875, 45.983334, 46.0, 46.0, 46.120834, 46.1625, 46.25, 46.483334, 46.5, 46.59375, 46.75, 46.758335, 46.758335, 46.98125, 47.0, 47.0, 47.04375, 47.085415, 47.385418, 47.5, 47.5, 47.75, 47.752083, 48.0, 48.0, 48.05, 48.091667, 48.410416, 48.5, 48.539585, 48.75, 48.760418, 49.0, 49.0, 49.004166, 49.022915, 49.25, 49.291668, 49.5, 49.5, 49.516666, 49.75, 49.770832, 50.0, 50.0, 50.0625, 50.066666, 50.25, 50.3125, 50.5, 50.510418, 50.529167, 50.75, 50.760418, 51.0, 51.0, 51.045834, 51.05, 51.354168, 51.5, 51.5, 51.75, 51.752083, 52.0, 52.0, 52.054165, 52.14375, 52.360416, 52.5, 52.54375, 52.75, 52.785416, 52.989582, 53.0, 53.0, 53.060417, 53.25, 53.30625, 53.5, 53.5, 53.558334, 53.75, 53.791668, 54.0, 54.0, 54.022915, 54.158333, 54.25, 54.302082, 54.5, 54.547916, 54.591667, 54.75, 54.802082, 54.852085, 54.939583, 54.960415, 54.960415, 55.0, 55.0, 55.0, 55.0, 55.0, 55.5, 56.0, 56.095833, 56.310417, 56.310417, 56.5, 56.522915, 56.522915, 56.608334, 56.608334, 56.785416, 56.991665, 57.0, 57.0, 57.085415, 57.497917, 57.5, 57.5, 58.0, 58.0, 58.00625, 58.070835, 58.5, 58.545834, 58.564583, 58.991665, 59.0, 59.0, 59.10625, 59.5, 60.0, 60.13125, 60.372917, 60.5, 60.5, 60.533333, 60.891666, 61.0, 61.0, 61.002083, 61.072918, 61.5, 61.5, 61.56875, 62.0, 62.0, 62.072918, 62.120834, 62.5, 62.55625, 62.59375, 63.0, 63.0, 63.008335, 63.085415, 63.5, 63.5, 63.533333, 64.0, 64.0, 64.04584, 64.09167, 64.5, 64.53958, 64.58125, 65.0, 65.0, 65.004166, 65.07916, 65.5, 65.60417, 66.0, 66.0, 66.0, 66.0, 66.066666, 66.52917, 66.839584, 66.86667, 66.87292, 67.0, 67.0, 67.0, 67.0, 67.05, 67.5, 68.0, 68.14375, 68.54375, 68.760414, 68.96667, 68.96875, 68.989586, 69.0, 69.0, 69.0, 69.0, 69.5, 70.0, 70.15833, 70.45625, 70.54792, 70.57083, 70.78125, 70.96042, 71.0, 71.0, 71.0, 71.0, 71.31042, 71.40625, 71.5, 71.5625, 72.0, 72.0, 72.0, 72.0, 72.0, 72.09583, 72.5, 72.60208, 73.0, 73.08542, 73.10208, 73.5, 73.57083, 73.59583, 73.68125, 74.0, 74.0, 74.0, 74.0, 74.0, 74.00625, 74.08125, 74.4625, 74.5, 74.56458, 74.625, 74.66875, 74.8625, 74.989586, 74.99167, 75.0, 75.0, 75.0, 75.0, 75.0, 75.5, 75.53333, 75.558334, 75.85208, 75.89167, 76.0, 76.0, 76.0, 76.13125, 76.5, 76.53333, 77.0, 77.00208, 77.072914, 77.5, 77.802086, 78.0, 78.0, 78.0, 78.0, 78.0, 78.07708, 78.120834, 78.5, 78.59375, 78.70417, 78.714584, 78.754166, 78.81458, 78.995834, 79.0, 79.0, 79.0, 79.0, 79.0, 79.08542, 79.5, 79.6625, 79.86875, 79.89375, 79.92917, 80.0, 80.0, 80.0, 80.09167, 80.5, 80.53958, 81.0, 81.004166, 81.020836, 81.5, 81.777084, 81.964584, 82.0, 82.0, 82.0, 82.0, 82.0, 82.066666, 82.5, 82.52917, 82.64167, 82.645836, 82.64792, 82.66458, 83.0, 83.0, 83.0, 83.0, 83.0, 83.05, 83.07083, 83.5, 84.0, 84.0, 84.00833, 84.035416, 84.041664, 84.05, 84.14375, 84.5, 84.5, 84.510414, 84.54375, 84.989586, 84.99167, 85.0, 85.0, 85.03125, 85.4125, 85.5, 85.5, 85.75, 85.76458, 86.0, 86.0, 86.15833, 86.25, 86.25833, 86.28125, 86.28125, 86.5, 86.5, 86.54792, 86.68542, 86.75, 86.9, 86.95625, 86.96042, 87.0, 87.0, 87.0, 87.0, 87.45625, 87.5, 87.52917, 87.55625, 88.0, 88.0, 88.0, 88.0, 88.0, 88.09583, 88.5, 88.53333, 88.60208, 88.64375, 88.770836, 88.83125, 89.0, 89.0, 89.0, 89.0, 89.0, 89.02292, 89.08542, 89.5, 89.575, 89.5875, 89.6, 89.61667, 90.0, 90.0, 90.0, 90.0, 90.00625, 90.5, 90.56458, 90.71042, 90.75, 90.81875, 90.81875, 90.895836, 90.93958, 90.99167, 91.0, 91.0, 91.0, 91.0, 91.20208, 91.5, 91.72708, 91.76458, 92.0, 92.0, 92.0, 92.0, 92.0, 92.13125, 92.5, 92.53333, 92.59375, 92.864586, 92.902084, 92.92292, 93.0, 93.0, 93.0, 93.0, 93.0, 93.00208, 93.072914, 93.5, 93.54375, 93.54792, 93.583336, 93.725, 94.0, 94.0, 94.0, 94.0, 94.120834, 94.5, 94.59375, 94.75, 94.785416, 94.785416, 94.90833, 94.933334, 94.958336, 95.0, 95.0, 95.0, 95.0, 95.08542, 95.5, 95.652084, 95.68958, 95.8875, 96.0, 96.0, 96.0, 96.0, 96.0, 96.09167, 96.5, 96.53958, 96.67917, 96.74167, 96.75, 97.0, 97.0, 97.0, 97.0, 97.0, 97.004166, 97.00625, 97.020836, 97.5, 97.7875, 97.864586, 97.879166, 97.93958, 98.0, 98.0, 98.0, 98.0, 98.0, 98.066666, 98.5, 98.52917, 98.73542, 98.73542, 98.84583, 98.8875, 98.93125, 99.0, 99.0, 99.0, 99.0, 99.0, 99.05, 99.5, 99.53125, 99.70625, 99.725, 99.98333, 100.0, 100.0, 100.0, 100.0, 100.0, 100.14375, 100.5, 100.54375, 100.589584, 100.660416, 100.73333, 100.989586, 100.99167, 100.99792, 101.0, 101.0, 101.0, 101.0, 101.0, 101.5, 101.66875, 101.70208, 101.70625, 101.902084, 102.0, 102.0, 102.0, 102.0, 102.0, 102.15833, 102.5, 102.73542, 102.90417, 102.941666, 102.95, 102.96042, 102.964584, 102.98542, 103.072914, 103.21667, 103.364586, 103.489586, 103.64375, 103.791664, 103.90417, 104.07083, 104.175, 104.879166, 110.722916, 110.739586, 110.7625, 110.8375, 110.85208, 110.85833, 110.86042, 110.879166};
bool infoList[] = {true, true, false, true, true, true, false, true, true, false, false, false, true, true, false, true, false, false, true, true, false, false, false, true, true, true, false, false, true, true, true, false, false, true, false, true, true, false, false, true, false, false, false, true, true, false, false, true, true, false, true, true, true, false, false, false, true, true, false, true, false, true, true, false, false, false, true, false, true, true, false, false, true, true, true, false, false, true, false, true, true, false, false, false, true, false, true, true, false, false, false, true, true, true, false, true, true, false, false, false, true, true, false, true, false, true, true, false, false, false, true, false, true, true, false, true, false, true, true, false, false, true, false, true, true, false, false, true, false, true, true, false, false, false, false, true, true, true, false, true, true, false, false, false, true, true, false, true, false, false, false, true, true, false, true, false, true, true, false, true, false, true, true, false, false, true, false, true, true, false, false, true, false, false, false, true, true, true, false, false, false, true, true, true, false, true, true, false, true, true, true, false, false, false, false, false, true, true, false, true, false, false, true, true, false, false, true, false, true, true, false, true, false, true, true, false, false, true, false, true, true, false, false, true, false, false, false, false, true, true, false, true, true, true, false, true, true, false, false, true, true, true, false, false, false, false, true, true, false, true, false, false, false, true, true, false, true, false, true, true, false, true, false, true, true, false, false, false, true, true, true, false, false, true, false, false, true, true, false, false, false, true, true, false, true, true, true, true, false, false, false, false, true, true, false, false, true, true, true, false, false, false, true, false, true, true, false, true, false, true, true, false, false, true, false, false, true, true, false, true, false, true, true, true, false, false, false, false, false, true, true, true, false, true, true, false, true, true, true, false, false, false, false, false, true, true, false, true, false, false, false, true, true, false, true, false, true, true, false, true, false, true, true, false, false, true, false, true, true, false, false, true, false, false, false, true, true, true, false, true, true, true, false, true, true, false, false, false, true, false, false, true, false, false, true, true, true, true, false, false, true, false, false, true, true, true, false, true, true, false, false, true, false, false, true, false, true, false, false, true, true, true, true, false, false, false, false, true, true, true, false, true, true, false, false, false, true, false, true, false, false, true, true, true, true, false, false, false, true, false, true, true, false, true, false, false, true, true, false, false, true, false, true, false, true, false, false, false, true, true, false, false, false, true, true, true, false, true, true, false, false, false, true, false, true, false, true, true, false, false, true, false, true, true, false, true, false, true, true, false, false, true, false, true, false, false, true, false, true, true, false, false, false, true, true, true, false, true, true, false, false, false, true, false, true, false, false, true, true, false, true, false, true, true, false, true, false, true, true, false, false, true, false, true, false, false, true, false, true, true, false, false, true, true, true, true, true, true, true, false, false, false, true, false, false, false, false, false, false, true, true, false, false, true, true, true, true, false, false, true, false, false, false, true, true, false, true, true, false, false, true, true, false, false, true, true, false, false, true, true, false, true, true, false, false, true, false, false, true, true, false, false, true, true, false, true, true, false, false, true, false, false, true, true, false, false, true, false, true, true, true, true, false, false, false, false, false, true, true, true, true, false, true, true, false, false, false, false, false, false, true, true, true, true, true, true, false, false, false, false, false, false, true, true, true, true, false, false, true, false, true, true, true, true, true, false, true, false, true, false, false, true, false, false, false, true, true, true, true, true, false, false, false, true, false, false, false, false, false, false, true, true, true, true, true, true, false, false, false, false, true, true, true, false, true, false, true, false, false, true, false, true, true, true, true, true, false, false, true, false, false, false, false, false, false, true, true, true, true, true, false, true, false, false, false, false, true, true, true, false, true, false, true, false, false, true, false, false, true, true, true, true, true, false, true, false, false, false, false, false, true, true, true, true, true, false, false, true, true, true, false, false, false, false, false, true, true, false, false, false, false, true, true, false, false, true, true, true, false, true, true, false, true, false, false, false, true, true, false, false, true, false, false, false, true, true, true, true, false, true, false, false, true, true, true, true, true, false, true, false, false, false, false, false, true, true, true, true, true, false, false, true, false, false, false, false, true, true, true, true, false, true, false, false, true, false, false, false, false, false, true, true, true, true, false, true, false, false, true, true, true, true, true, false, true, false, false, false, false, false, true, true, true, true, true, false, false, true, false, false, false, false, true, true, true, true, false, true, false, true, false, false, false, false, false, true, true, true, true, false, true, false, false, false, true, true, true, true, true, false, true, false, false, false, false, true, true, true, true, true, false, false, false, true, false, false, false, false, true, true, true, true, true, false, true, false, false, false, false, false, false, true, true, true, true, true, false, true, false, false, false, false, true, true, true, true, true, false, true, false, false, false, false, false, false, false, true, true, true, true, true, true, false, false, false, false, true, true, true, true, true, false, true, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false};
int freqList[] = {D3, A5, A5, A3, FS5, G5, FS5, D4, A5, G5, A3, A5, FS4, FS5, D4, G5, FS5, G5, A2, A5, FS4, D3, A5, A4, CS4, B4, A4, B4, CS5, E4, D5, CS4, CS5, E5, D5, A4, FS5, E5, E4, G5, FS5, A4, A2, B2, FS5, G5, FS5, FS3, D5, D5, E5, B3, FS5, E5, FS3, FS5, D4, FS4, B3, G4, FS4, FS2, A4, D4, G4, B2, B4, A4, A3, A4, B4, A4, G4, CS4, A4, G4, A3, FS4, A4, FS4, G4, FS4, FS4, CS4, A4, G4, G2, G4, A4, FS2, G4, D3, B4, A4, B4, G3, G4, A4, D3, G4, B3, FS4, G3, E4, FS4, D2, FS4, G2, E4, B3, E4, FS4, FS3, D4, E4, E4, D4, A3, FS4, E4, FS3, G4, FS4, D4, A4, A3, G4, B4, A4, G2, G4, B4, D2, D4, G4, E3, B4, A4, B4, B3, B4, A4, E3, B4, D4, CS5, B3, D5, CS5, G2, D4, A2, A4, D5, B4, A4, E3, CS5, B4, D5, CS5, A3, E5, D5, E3, FS5, E5, CS4, G5, FS5, A3, A5, G5, CS4, A2, D3, D5, FS5, A5, D5, FS5, A3, D5, FS5, D5, E5, G5, FS5, D4, FS5, A5, E5, G5, A3, FS5, A5, FS4, E5, D4, D5, E5, D5, A2, E5, FS4, D3, CS5, E5, CS4, D5, CS5, E5, D5, E4, FS5, CS4, E5, E5, FS5, A4, D5, E5, E4, CS5, D5, CS5, A4, A2, B2, D5, D5, FS3, B4, D5, D5, CS5, E5, B4, E5, B3, D5, FS5, CS5, FS3, D5, FS5, D4, D4, B3, E4, D4, D4, E4, FS2, FS4, B2, G4, FS4, A3, FS4, G4, E4, FS4, CS4, FS4, E4, A3, FS4, D5, FS4, CS5, D5, CS4, D5, CS5, FS4, G2, B4, D5, FS2, B4, D3, D5, D5, CS5, G3, G4, B4, CS5, D3, G4, B4, B3, A4, G3, A4, G4, D2, A4, G2, G4, B3, G4, A4, FS3, FS4, G4, G4, FS4, A3, A4, FS3, G4, B4, A4, B4, D4, CS5, A3, D5, CS5, G2, G4, B4, D5, D2, D4, B4, G4, E3, B4, D5, D5, A4, CS5, B4, B3, B4, D5, A4, CS5, E3, D5, B4, D4, CS5, B3, B4, CS5, G2, D4, A2, CS5, B4, D5, CS5, E3, E5, D5, D5, E5, A3, CS5, D5, E3, D5, CS5, CS4, B4, A3, D5, CS5, B4, CS4, A2, D3, D5, FS5, CS5, A3, D4, E4, D4, D4, FS4, E4, A3, FS4, E4, D4, E4, D4, FS5, D5, A2, E4, CS5, E5, D4, D3, CS4, E4, CS4, E3, D4, E4, D4, A3, FS4, E3, E4, E4, FS4, E4, D4, A3, CS4, D4, A2, B2, D4, B4, D5, CS4, CS5, E5, D4, D3, B3, CS4, B3, FS3, D4, CS4, D3, D4, D4, FS3, E4, D4, D5, FS2, FS4, A4, CS5, B4, E4, B2, G4, FS4, CS3, FS4, G4, E4, FS4, E4, FS3, D4, CS3, D4, D5, D5, CS5, FS3, D5, CS5, CS5, A4, G2, B4, D5, FS2, B4, D3, D5, CS5, D5, G3, B4, CS5, D3, B4, A4, G3, G4, A4, D2, A4, G2, G4, G4, A4, FS3, FS4, G4, G4, FS4, A3, A4, G4, FS3, B4, A4, CS5, B4, A3, D5, CS5, G2, B4, D5, D2, B4, E3, D5, CS5, D5, B3, D5, CS5, E3, D5, CS5, B3, B4, CS5, G2, A2, CS5, B4, D5, CS5, E3, E5, D5, D5, E5, A3, CS5, D5, E3, D5, CS5, B4, A3, D5, CS5, B4, FS4, A4, A2, CS5, D3, D4, FS4, A4, D5, A3, D4, A3, A4, A4, A4, D4, D4, FS4, FS4, A4, D5, A2, A4, D3, A4, CS4, B4, E4, A4, CS4, B4, G4, A4, E4, A2, B2, FS4, G4, FS3, B3, FS3, FS4, D4, FS4, B3, FS4, FS2, FS4, D4, B2, A3, G4, FS4, CS4, FS4, G4, A3, E4, FS4, CS4, G2, D4, E4, FS2, D3, C4, D4, G3, B3, C4, D3, C4, G3, B3, D2, A3, G2, C4, FS3, A3, A3, D4, FS4, A4, FS3, A3, D4, A4, FS4, G2, D4, G4, B4, D2, E3, B3, E3, B3, D4, G4, B4, G2, A2, CS4, E4, A4, E3, A3, E3, E4, A3, CS4, A4, A2, D3, FS4, A4, D5, A4, D5, A3, FS4, D4, D5, FS5, A5, D6, A3, FS4, D4, A2, D3, FS4, CS4, D5, FS5, A5, E4, CS5, E5, A5, CS6, CS4, D6, CS5, A4, E4, E5, A5, CS6, A4, A2, B2, D5, FS5, B5, D6, FS3, D5, B5, FS5, D6, B3, B4, B5, FS3, D4, B3, FS2, D4, B2, A3, B4, CS4, A4, C5, FS5, A5, B5, A3, FS4, CS4, A4, FS5, C5, A5, FS4, G2, A4, B4, D5, A5, FS2, D3, A4, B4, D5, A5, G3, G4, G5, D3, B3, G3, D2, G2, B3, FS3, G4, G5, A3, FS4, A4, D5, FS5, FS3, D4, A3, FS4, A4, D5, FS5, G2, FS4, A4, D5, FS5, D2, D4, E3, B3, E5, D5, FS5, FS4, A4, E3, D4, D5, E5, B3, G2, D4, A2, CS5, D5, CS5, E3, D5, CS5, D5, A3, D5, E3, CS5, D5, CS5, CS5, CS4, B4, A3, B4, CS5, CS4, CS5, A2, D3, FS4, A4, D5, D5, A3, FS4, A4, D4, D5, FS5, A5, D6, A3, FS4, A5, D4, D5, D6, FS5, A2, CS5, E5, A5, CS6, FS4, D3, CS4, E5, CS6, A5, CS5, E4, A4, CS5, E5, CS4, A4, E4, CS5, G4, A4, A4, G4, E5, A2, B2, FS4, B4, D5, B4, FS3, D5, FS4, B3, B4, D5, FS5, B5, FS3, D4, B3, D5, B5, B4, FS5, FS2, A4, CS5, E5, A5, D4, B2, A3, A5, CS5, E5, A4, CS4, FS4, A4, CS5, A3, FS4, CS4, E4, FS4, FS4, A4, CS5, E4, G2, D4, G4, B4, FS2, D3, B4, G4, D4, G3, G4, B4, D5, G5, D3, B3, G3, D5, B4, G5, D2, FS4, A4, D5, FS5, G2, G4, B3, FS3, D5, A4, FS4, FS5, A3, D4, FS4, A4, D5, FS3, D4, A3, D4, D4, A4, FS4, D5, G2, E4, G4, B4, E5, D2, E3, E5, G4, B4, E4, B3, G4, B4, E5, G5, E3, D4, B3, E5, G4, B4, G2, D4, G5, A2, E4, A4, CS5, E5, E3, E5, CS5, A4, E4, A3, A4, CS5, E5, A5, E3, CS4, A3, CS5, A5, E5, A2, A4, CS4, D2, D3, FS3, A3, D4, D5, FS5, A5, D6, D5, D6, A5, FS5, D3, D4, A3, FS3, D2};
int played = -1;

Note notes[musicSize];

void setup() {
  M5.begin();
  M5.Power.begin();
  Wire.begin();
  imu.initMPU9250();
  M5.Lcd.setTextSize(3);
  for (int i = 0; i < musicSize; i++) {
    notes[i] = Note(timeList[i], infoList[i], freqList[i]);
  }
  draw();
}

void loop() {
  M5.update();
  play();
  buttonDetect();
}

void draw() {
  //M5.Lcd.fillRect(65, 190, 265, 230, BLACK);
  M5.Lcd.clear();
  switch (displayMode) {
    case 0:
      M5.Lcd.fillRect(145, 195, 30, 30, WHITE);
      M5.Lcd.fillCircle(70, 210, 5, WHITE);
      M5.Lcd.fillCircle(70, 195, 5, WHITE);
      M5.Lcd.fillCircle(70, 225, 5, WHITE);
      M5.Lcd.fillTriangle(235, 195, 235, 225, 260, 210, WHITE);
      break;
    case 1:
      M5.Lcd.fillRect(145, 195, 30, 30, WHITE);
      M5.Lcd.fillCircle(70, 210, 5, WHITE);
      M5.Lcd.fillCircle(70, 195, 5, WHITE);
      M5.Lcd.fillCircle(70, 225, 5, WHITE);
      M5.Lcd.fillRect(235, 195, 10, 30, WHITE);
      M5.Lcd.fillRect(255, 195, 10, 30, WHITE);
      break;
    case 2:
      M5.Lcd.fillCircle(160, 210, 15, WHITE);
      M5.Lcd.fillTriangle(85, 195, 55, 195, 70, 225, WHITE);
      M5.Lcd.fillTriangle(235, 225, 265, 225, 250, 195, WHITE);
      M5.Lcd.setCursor(0, 100);
      M5.Lcd.setTextSize(2);
      M5.Lcd.print("Hold B to enter\r\nShake Adjust Mode.");
      M5.Lcd.setCursor(0, 150);
      M5.Lcd.print("And hold A and C to\r\nreturn to Playback Mode.");
      break;
  }
  M5.Lcd.setCursor(80, 50);
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("BPM:");
  M5.Lcd.print(bpm);
}

void buttonDetect() {
  switch (displayMode) {
    case 0:
    case 1:
      if (M5.BtnA.wasPressed()) {
        stopPlaying();
        displayMode = 2;
        draw();
      }
      if (M5.BtnB.wasPressed()) {
        stopPlaying();
      }
      if (M5.BtnC.wasPressed()) {
        pausePlaying(displayMode == 1);
        displayMode = 1 - displayMode;
        draw();
      }
      break;
    case 2:
      if (M5.BtnA.wasPressed() || M5.BtnA.pressedFor(1000, 200)) {
        bpm--;
        beatTime = 60000000 / bpm;
        draw();
      }
      if (M5.BtnB.read()) {
        calcBPM();
      }
      if (M5.BtnC.wasPressed() || M5.BtnC.pressedFor(1000, 200)) {
        bpm++;
        beatTime = 60000000 / bpm;
        draw();
      }
      if(M5.BtnA.read() && M5.BtnC.read()) {
        displayMode = 0;
        M5.Lcd.clear();
        draw();
      }
      break;
  }
}

void play() {
  if (nowPlaying) {
    long playbackTime = micros() - startTime;
    for (int i = played + 1; i < musicSize; i++) {
      if (notes[i].getTime() * beatTime < playbackTime) {
        if (notes[i].getInfo()) {
          tones.addNote(notes[i].getFreq());
        } else {
          tones.delNote(notes[i].getFreq());
        }
        played++;
      } else {
        break;
      }
    }
    dacWrite(speakerPin, tones.calcWave());
  } else {
    dacWrite(speakerPin, 0);
  }
}

void stopPlaying() {
  nowPlaying = false;
  started = false;
  displayMode = 0;
  draw();
}

void pausePlaying(bool b) {
  switch (b) {
    case true:
      pausedTime = micros();
      nowPlaying = false;
      break;
    case false:
      if (started) {
        startTime += (micros() - pausedTime);
        nowPlaying = true;
      } else {
        startPlaying();
      }
  }
}

void startPlaying() {
  started = true;
  played = -1;
  tones.clear();
  startTime = micros();
  nowPlaying = true;
}

void calcBPM() {
  if (isAcceling(30)) {
    if (!countFlag) {
      M5.Lcd.fillScreen(RED);
      countFlag = true;
      long tmpNowTime = micros();
      long elapse = tmpNowTime - offTime;
      rhythms.push(elapse);
      offTime = tmpNowTime;
    }
  } else {
    if (countFlag) {
      M5.Lcd.fillScreen(BLACK);
      countFlag = false;
      draw();
      beatTime = rhythms.getAverage();
      if (beatTime == 0) beatTime = 500000;
      bpm = beatTime != 0 ? 60000000 / beatTime : 0;
    }
  }
}

int getAccel() {
  if (imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {
    imu.readAccelData(imu.accelCount);
    imu.getAres();
    imu.ax = (float)imu.accelCount[0] * imu.aRes * 20;
    imu.ay = (float)imu.accelCount[1] * imu.aRes * 20;
    imu.az = (float)imu.accelCount[2] * imu.aRes * 20;
    return (int)sqrt(imu.ax * imu.ax + imu.ay * imu.ay + imu.az * imu.az);
  }
  return 0;
}

bool isAcceling(int hold) {
  if (getAccel() > hold) {
    return true;
  }
  return false;
}

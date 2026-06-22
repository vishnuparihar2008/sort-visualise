#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600
#define MAX_COUNT 500

int *numbers = NULL;
int COUNT = 0;

typedef struct {
  bool swapped;   // a swap happened this step
  bool comparing; // two elements are being actively compared
  int i, j;       // to highlight indices
  bool done;
} SortStatus;

typedef struct {
  const char *name;
  SortStatus (*step)(void);
  void (*reset)(void);
} Algorithm;

static void swapEl(int a, int b) {
  int t = numbers[a];
  numbers[a] = numbers[b];
  numbers[b] = t;
}

static void shuffleNumbers(void) {
  for (int i = 0; i < COUNT; i++)
    numbers[i] = i + 1; /* 1-based so no zero-height bar */
  for (int i = COUNT - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    swapEl(i, j);
  }
}

static void drawBars(SortStatus s, const char *name, int spf) {
  float barW = (float)SCREEN_WIDTH / COUNT;
  int baseY = SCREEN_HEIGHT - 40;
  int maxH = baseY - 40;

  for (int i = 0; i < COUNT; i++) {
    int h = (int)((float)numbers[i] / COUNT * maxH);
    Color c = WHITE;
    if (s.done)
      c = LIME;
    else if (i == s.i || i == s.j)
      c = s.swapped ? RED : (s.comparing ? ORANGE : WHITE);

    DrawRectangle((int)(i * barW), baseY - h, (int)(barW - 1), h, c);
  }

  // Top
  DrawText(name, 10, 10, 20, RAYWHITE);
  char spfBuf[64];
  snprintf(spfBuf, sizeof spfBuf, "Speed: %d  [UP/DN]", spf);
  DrawText(spfBuf, SCREEN_WIDTH - 200, 10, 16, GRAY);

  // Bottom
  const char *hint =
      s.done ? "Done!   R=reshuffle   1/2/3=algorithm"
             : "1:Bubble  2:Selection  3:Insertion   R:Reshuffle   UP/DN:Speed";
  DrawText(hint, 10, SCREEN_HEIGHT - 28, 16, s.done ? LIME : DARKGRAY);
}

// Bubble Sort
static int bs_i = 0;
static bool bs_anySwap = false;

static void bubbleReset(void) {
  bs_i = 0;
  bs_anySwap = false;
}

static SortStatus bubbleStep(void) {
  SortStatus s = {false, false, bs_i, bs_i + 1, false};
  if (bs_i < COUNT - 1) {
    s.comparing = true;
    if (numbers[bs_i] > numbers[bs_i + 1]) {
      swapEl(bs_i, bs_i + 1);
      s.swapped = true;
      bs_anySwap = true;
    }
    bs_i++;
  } else {
    if (!bs_anySwap)
      s.done = true;
    bs_i = 0;
    bs_anySwap = false;
  }
  return s;
}

// Selection Sort
static int ss_i = 0, ss_j = 1, ss_min = 0;

static void selectionReset(void) {
  ss_i = 0;
  ss_j = 1;
  ss_min = 0;
}

static SortStatus selectionStep(void) {
  if (ss_i >= COUNT - 1)
    return (SortStatus){false, false, ss_i, ss_i, true};

  SortStatus s = {false, false, ss_min, ss_j, false};

  if (ss_j < COUNT) {
    // scanning: find minimum in unsorted portion
    s.i = ss_min;
    s.j = ss_j;
    s.comparing = true;
    if (numbers[ss_j] < numbers[ss_min])
      ss_min = ss_j;
    ss_j++;
  } else {
    // place: swap found minimum into sorted position
    s.i = ss_i;
    s.j = ss_min;
    if (ss_min != ss_i) {
      swapEl(ss_i, ss_min);
      s.swapped = true;
    }
    ss_i++;
    ss_min = ss_i;
    ss_j = ss_i + 1;
  }
  return s;
}

// Insertion Sort
static int is_i = 1, is_j = 1;

static void insertionReset(void) {
  is_i = 1;
  is_j = 1;
}

static SortStatus insertionStep(void) {
  if (is_i >= COUNT)
    return (SortStatus){false, false, 0, 0, true};

  // highlight the pair being examined
  SortStatus s = {
      .swapped = false,
      .comparing = is_j > 0,
      .i = is_j > 0 ? is_j - 1 : 0,
      .j = is_j,
      .done = false,
  };

  if (is_j > 0 && numbers[is_j - 1] > numbers[is_j]) {
    swapEl(is_j - 1, is_j);
    s.swapped = true;
    is_j--;
  } else {
    /*
     * element is in place;
     * move to next unsorted element
     */
    is_i++;
    is_j = is_i;
  }
  return s;
}

// Algorithm Table
static const Algorithm algos[] = {
    {"Bubble Sort", bubbleStep, bubbleReset},
    {"Selection Sort", selectionStep, selectionReset},
    {"Insertion Sort", insertionStep, insertionReset},
};
#define ALGO_COUNT ((int)(sizeof algos / sizeof algos[0]))

static const SortStatus INIT_STATUS = {false, false, 0, 1, false};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr,
            "Usage: %s <count> [algo]\n"
            "  count : 2 – %d\n"
            "  algo  : 1=Bubble  2=Selection  3=Insertion  (default 1)\n",
            argv[0], MAX_COUNT);
    return 1;
  }

  COUNT = atoi(argv[1]);
  if (COUNT < 2 || COUNT > MAX_COUNT) {
    fprintf(stderr, "error: count must be 2–%d\n", MAX_COUNT);
    return 1;
  }

  int algoIdx = 0;
  if (argc >= 3) {
    int a = atoi(argv[2]) - 1;
    if (a >= 0 && a < ALGO_COUNT)
      algoIdx = a;
  }

  numbers = malloc(COUNT * sizeof *numbers);
  if (!numbers) {
    perror("malloc");
    return 1;
  }

  srand((unsigned)time(NULL));
  shuffleNumbers();
  algos[algoIdx].reset();

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sorting Visualiser");
  SetTargetFPS(60);

  SortStatus status = INIT_STATUS;
  int stepsPerFrame = (COUNT > 50) ? COUNT / 10 : 1;

  while (!WindowShouldClose()) {

    // Input while Sorting
    if (IsKeyPressed(KEY_R)) {
      shuffleNumbers();
      algos[algoIdx].reset();
      status = INIT_STATUS;
    }
    for (int k = 0; k < ALGO_COUNT; k++) {
      if (IsKeyPressed(KEY_ONE + k)) {
        algoIdx = k;
        shuffleNumbers();
        algos[algoIdx].reset();
        status = INIT_STATUS;
      }
    }
    if (IsKeyPressed(KEY_UP) && stepsPerFrame < COUNT)
      stepsPerFrame++;
    if (IsKeyPressed(KEY_DOWN) && stepsPerFrame > 1)
      stepsPerFrame--;

    // Steps
    for (int n = 0; n < stepsPerFrame && !status.done; n++)
      status = algos[algoIdx].step();

    BeginDrawing();
    ClearBackground(BLACK);
    drawBars(status, algos[algoIdx].name, stepsPerFrame);
    EndDrawing();
  }

  CloseWindow();
  free(numbers);
  return 0;
}

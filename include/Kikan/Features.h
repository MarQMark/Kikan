#ifndef KIKAN_FEATURES_H
#define KIKAN_FEATURES_H

#define KIKAN_RENDERER 1

#define KIKAN_ECS 1

#define KIKAN_INPUT 1

#define KIKAN_UI 1

// Input works with window which is part of renderer
#if !KIKAN_RENDERER
#undef KIKAN_INPUT
#define KIKAN_INPUT 0
#endif

#if !KIKAN_RENDERER
#undef KIKAN_UI
#define KIKAN_UI 0
#endif

#if !KIKAN_INPUT
#undef KIKAN_UI
#define KIKAN_UI 0
#endif

#endif //KIKAN_FEATURES_H

#include "raylib.h"
#include "musica.h"
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void inicializar_notas();
void inicializar_notas_oni();
void notas_cancion2();
void notas_cancion2_oni();
void notas_cancion3();
void notas_cancion3_oni();
void momento_golpe(double tiempo, double *tiempo_golpe, Music musica, int max_notas);
int detectar_golpes(double *tiempo_golpe, Music musica);
void puntuaciones(int puntos);
void pantalla_inicio();
int menu();
void cancion1(int dificultad);
void cancion2(int dificultad);
void cancion3(int dificultad);
void void_song();
void records_menu(int cancion, int dificultad);
float easeOutBounce(float t);
void pause(Music musica);
void dibujar_interfaz_completa(int cancion, float tiempo_musica, int score, int combo);
void mostrar_log(const char *mensaje);
void pantalla_resultados(int cancion, int dificultad);
void dibujarMenuPausa();

#define MAX_NOTAS 4000
Ts_notas notas[MAX_NOTAS];
int score = 0;
int combo = 0;
bool stop = false;
int opcionMenuPausa = 0;
Music musica_inicio;
Pantalla Pantalla_actual = INICIO;
Font miFuente;
char ultimo_log[64] = "";
float log_timer = 0.0f;
Resultados resultados_actuales = {0, 0, 0, 0, 0, 0};
int cancion_actual = 0;
int dificultad_actual = 0;

Stats_Cancion stats_cancion1 = {0, 0, 0, 0};
Stats_Cancion stats_cancion2 = {0, 0, 0, 0};
Stats_Cancion stats_cancion3 = {0, 0, 0, 0};

int main()
{
    SetExitKey(KEY_NULL);
    srand(time(NULL));
    int dificultad = 1;
    InitWindow(1920, 1080, "TAIKO");
    SetTraceLogLevel(LOG_INFO);
    InitAudioDevice();
    SetTargetFPS(153);
    musica_inicio = LoadMusicStream("assets/music/musica_inicio.ogg");
    PlayMusicStream(musica_inicio);

    while (!WindowShouldClose())
    {
        switch (Pantalla_actual)
        {
        case INICIO:
        {
            pantalla_inicio();
            break;
        }

        case MENU_CANCIONES:
        {
            stop = false;
            score = 0;
            combo = 0;

            dificultad = menu();
            break;
        }

        case CANCION1:
        {

            cancion1(dificultad);
            break;
        }

        case CANCION2:
        {

            cancion2(dificultad);
            break;
        }

        case CANCION3:
        {

            cancion3(dificultad);
            break;
        }

        case RESULTADOS:
        {
            pantalla_resultados(cancion_actual, dificultad_actual);
            break;
        }
        }
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void pantalla_inicio()
{
    SetExitKey(KEY_NULL);
    static int cargado = 0;
    static Texture2D logo;
    static Texture2D fondo;
    static Texture2D sprite1;
    static Texture2D sprite2;
    static Texture2D *spriteActual;
    static Texture2D sprite3;
    static Texture2D sprite4;
    static Sound entrada;
    Color rojo_taiko;
    static float timer = 0;
    float duracion_sprite1 = 0.7f;
    float duracion_sprite2 = 0.7f;
    static int animando = 0;
    static float t_anim = 0;
    float dur_anim = 0.8f;
    float pausa = 1.0f;

    if (cargado == 0)
    {
        rojo_taiko = (Color){230, 0, 18, 255};
        sprite1 = LoadTexture("assets/images/inicio_frame01.png");
        sprite2 = LoadTexture("assets/images/inicio_frame2.png");
        sprite3 = LoadTexture("assets/images/personajes_inicio.png");
        sprite4 = LoadTexture("assets/images/personajes2_inicio.png");
        fondo = LoadTexture("assets/images/fondo_principal.png");
        miFuente = LoadFontEx("assets/fonts/fuente_juego.ttf", 72, NULL, 0);
        GenTextureMipmaps(&miFuente.texture);
        SetTextureFilter(miFuente.texture, TEXTURE_FILTER_BILINEAR);
        logo = LoadTexture("assets/images/TAIKO_LOGO.png");
        entrada = LoadSound("assets/sounds/tuturu.mp4");
        spriteActual = &sprite1;
        cargado = 1;
    }

    UpdateMusicStream(musica_inicio);
    BeginDrawing();
    ClearBackground(BLACK);

    float dt = GetFrameTime();
    timer += dt;

    if (spriteActual == &sprite1 && timer >= duracion_sprite1)
    {
        spriteActual = &sprite2;
        timer = 0;
    }

    if (spriteActual == &sprite2 && timer >= duracion_sprite2)
    {
        spriteActual = &sprite1;
        timer = 0;
    }

    DrawTexturePro(
        fondo,
        (Rectangle){0, 0, 1024, 1024},
        (Rectangle){0, 0, 1920, 1080},
        (Vector2){0, 0},
        0.0f,
        WHITE);
    DrawTextureEx(logo, (Vector2){525, -120}, 0.0f, 0.69f, WHITE);

    if (spriteActual == &sprite1)
    {
        DrawTextureEx(*spriteActual, (Vector2){485, 525}, 0.0f, 0.825f, WHITE);
    }
    else
    {
        DrawTextureEx(*spriteActual, (Vector2){395, 495}, 0.0f, 0.975f, WHITE);
    }
    DrawTextEx(miFuente, "PRESIONA PARA COMENZAR", (Vector2){515, 975}, 60, 3, rojo_taiko);

    if (animando)
    {
        t_anim += GetFrameTime();

        float p = t_anim / dur_anim;
        if (p > 1.0f)
            p = 1.0f;

        float bounce = sinf(p * 3.0f) * (1.0f - p) + p;

        Vector2 posIzqFinal = {0, 375};
        Vector2 posDerFinal = {805, 375};

        Vector2 posIzq = posIzqFinal;
        Vector2 posDer = posDerFinal;

        posIzq.x = -600 + bounce * (posIzqFinal.x + 600);
        posDer.x = 1800 - bounce * (1800 - posDerFinal.x);

        DrawTextureEx(sprite3, posIzq, 0, 1.05f, WHITE);
        DrawTextureEx(sprite4, posDer, 0, 1.05f, WHITE);
        if (t_anim > dur_anim + pausa)
        {
            EndDrawing();
            Pantalla_actual = MENU_CANCIONES;
            return;
        }
    }

    if (!animando && (GetKeyPressed() || IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)))
    {
        animando = 1;
        t_anim = 0;
        PlaySound(entrada);
    }

    EndDrawing();
}

int menu()
{
    SetExitKey(KEY_NULL);
    UpdateMusicStream(musica_inicio);
    static int cargado = 0;
    static int cancion_wed = 0;
    static int cancion2 = 0;
    static int cancion3 = 0;
    static Music musica1;
    static Music musica2;
    static Music musica3;
    static int dificultad; // 1 = NORMAL, 2 = ONI
    static bool mostrar_dif;
    static Texture2D estrella1;
    static Texture2D estrella2;
    static Texture2D sprite1;
    static Texture2D sprite2;
    static Texture2D sprite3;
    static Texture2D *spriteActual;
    static float timer = 0;

    if (cargado == 0)
    {
        dificultad = 1;
        bool mostrar_dif = false;
        musica1 = LoadMusicStream("assets/music/dominator.mp3");
        musica2 = LoadMusicStream("assets/music/sun.ogg");
        musica3 = LoadMusicStream("assets/music/p3.ogg");
        estrella1 = LoadTexture("assets/images/estrella1.png");
        estrella2 = LoadTexture("assets/images/estrella2.png");
        sprite1 = LoadTexture("assets/images/sprite1_menu.png");
        sprite2 = LoadTexture("assets/images/sprite2_menu.png");
        sprite3 = LoadTexture("assets/images/sprite3_menu.png");
        spriteActual = &sprite1;
        cargado = 1;
    }

    Color rojo = (Color){230, 0, 18, 255};
    Color gris = (Color){70, 70, 70, 255};

    BeginDrawing();
    ClearBackground(BLACK);

    float dt = GetFrameTime();
    timer += dt;

    for (int i = -300; i < 1800; i += 60)
        DrawLine(i, 0, i + 300, 1080, (Color){255, 255, 255, 20});

    DrawRectanglePro((Rectangle){-90, 0, 1050, 345},
                     (Vector2){0, 0}, -12, rojo);

    Rectangle hit_cancion1 = {553, 375, 915, 165};
    Rectangle hit_cancion2 = {553, 600, 915, 165};
    Rectangle hit_cancion3 = {553, 825, 915, 165};

    DrawRectanglePro((Rectangle){548, 375, 930, 180},
                     (Vector2){465, 90}, -12.0f, WHITE);

    DrawRectanglePro(hit_cancion1,
                     (Vector2){465, 82.5f}, -12.0f, (cancion_wed == 1) ? rojo : gris);

    DrawTextPro(miFuente, "WORLD END DOMINATOR",
                (Vector2){132, 432}, (Vector2){0, 0},
                -12.0f, 68.0f, 3.0f, BLACK);

    DrawTextPro(miFuente, "WORLD END DOMINATOR",
                (Vector2){127, 427}, (Vector2){0, 0},
                -12.0f, 68.0f, 3.0f, WHITE);

    DrawRectanglePro((Rectangle){548, 600, 930, 180},
                     (Vector2){465, 90}, -12.0f, WHITE);

    DrawRectanglePro(hit_cancion2,
                     (Vector2){465, 82.5f}, -12.0f, (cancion2 == 1) ? rojo : gris);

    DrawTextPro(miFuente, "SUN",
                (Vector2){132, 665}, (Vector2){0, 0},
                -12.0f, 68.0f, 3.0f, BLACK);

    DrawTextPro(miFuente, "SUN",
                (Vector2){127, 660}, (Vector2){0, 0},
                -12.0f, 68.0f, 3.0f, WHITE);

    DrawRectanglePro((Rectangle){548, 825, 930, 180},
                     (Vector2){465, 90}, -12.0f, WHITE);

    DrawRectanglePro(hit_cancion3,
                     (Vector2){465, 82.5f}, -12.0f, (cancion3 == 1) ? rojo : gris);

    DrawTextPro(miFuente, "COLOR YOUR NIGHT",
                (Vector2){132, 890}, (Vector2){0, 0},
                -12.0f, 68.0f, 3.0f, BLACK);

    DrawTextPro(miFuente, "COLOR YOUR NIGHT",
                (Vector2){127, 885}, (Vector2){0, 0},
                -12.0f, 68.0f, 3.0f, WHITE);

    if ((cancion_wed == 0 && cancion2 == 0 && cancion3 == 0) && GetKeyPressed())
    {
        StopMusicStream(musica_inicio);
        cancion_wed = 1;
        cancion2 = 0;
        cancion3 = 0;
    }

    if (cancion_wed == 1 && (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && mostrar_dif == false)
    {
        cancion_wed = 0;
        cancion2 = 1;
        cancion3 = 0;
    }
    else
    {
        if (cancion2 == 1 && (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && mostrar_dif == false)
        {
            cancion_wed = 1;
            cancion2 = 0;
            cancion3 = 0;
        }
        else
        {
            if (cancion2 == 1 && (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && mostrar_dif == false)
            {
                cancion_wed = 0;
                cancion2 = 0;
                cancion3 = 1;
            }
            else
            {
                if (cancion3 == 1 && (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && mostrar_dif == false)
                {
                    cancion_wed = 0;
                    cancion2 = 1;
                    cancion3 = 0;
                }
            }
        }
    }

    if (cancion_wed == 1)
    {
        DrawTextPro(miFuente, "WORLD END DOMINATOR",
                    (Vector2){50, 184}, (Vector2){0, 0},
                    -12.0f, 75.0f, 3.0f, BLACK);

        DrawTextPro(miFuente, "WORLD END DOMINATOR",
                    (Vector2){45, 180}, (Vector2){0, 0},
                    -12.0f, 75.0f, 3.0f, WHITE);
    }
    else if (cancion2 == 1)
    {
        DrawTextPro(miFuente, "SUN",
                    (Vector2){50, 184}, (Vector2){0, 0},
                    -12.0f, 75.0f, 3.0f, BLACK);

        DrawTextPro(miFuente, "SUN",
                    (Vector2){45, 180}, (Vector2){0, 0},
                    -12.0f, 75.0f, 3.0f, WHITE);
    }
    else if (cancion3 == 1)
    {
        DrawTextPro(miFuente, "COLOR YOUR NIGHT",
                    (Vector2){50, 184}, (Vector2){0, 0},
                    -12.0f, 75.0f, 3.0f, BLACK);

        DrawTextPro(miFuente, "COLOR YOUR NIGHT",
                    (Vector2){45, 180}, (Vector2){0, 0},
                    -12.0f, 75.0f, 3.0f, WHITE);
    }
    else
    {
        DrawTextPro(miFuente, "SELECCIONA UNA CANCION",
                    (Vector2){50, 184}, (Vector2){0, 0},
                    -12.0f, 68.0f, 3.0f, BLACK);

        DrawTextPro(miFuente, "SELECCIONA UNA CANCION",
                    (Vector2){45, 180}, (Vector2){0, 0},
                    -12.0f, 68.0f, 3.0f, WHITE);
    }
    DrawTextureEx(estrella1, (Vector2){1420, 700}, 0.0f, 0.75f, WHITE);
    DrawTextureEx(estrella2, (Vector2){1450, 723}, 0.0f, 0.705f, rojo);

    if (spriteActual == &sprite1 && timer >= 0.7f)
    {
        spriteActual = &sprite2;
        timer = 0;
    }

    if (spriteActual == &sprite2 && timer >= 0.7f)
    {
        spriteActual = &sprite3;
        timer = 0;
    }

    if (spriteActual == &sprite3 && timer >= 0.7f)
    {
        spriteActual = &sprite1;
        timer = 0;
    }

    if (spriteActual == &sprite1)
    {
        DrawTextureEx(*spriteActual, (Vector2){1642, 760}, 0.0f, 0.3f, WHITE);
    }
    else
    {
        if (spriteActual == &sprite2)
        {
            DrawTextureEx(*spriteActual, (Vector2){1642, 760}, 0.0f, 0.3f, WHITE);
        }
        else
        {
            if (spriteActual == &sprite3)
            {
                DrawTextureEx(*spriteActual, (Vector2){1642, 760}, 0.0f, 0.315f, WHITE);
            }
        }
    }

    if (cancion_wed == 1)
    {
        if (!IsMusicStreamPlaying(musica1))
            PlayMusicStream(musica1);
        UpdateMusicStream(musica1);

        StopMusicStream(musica2);
        StopMusicStream(musica3);
    }
    else if (cancion2 == 1)
    {
        if (!IsMusicStreamPlaying(musica2))
            PlayMusicStream(musica2);
        UpdateMusicStream(musica2);

        StopMusicStream(musica1);
        StopMusicStream(musica3);
    }
    else if (cancion3 == 1)
    {
        if (!IsMusicStreamPlaying(musica3))
            PlayMusicStream(musica3);
        UpdateMusicStream(musica3);

        StopMusicStream(musica1);
        StopMusicStream(musica2);
    }
    else
    {
        StopMusicStream(musica1);
        StopMusicStream(musica2);
        StopMusicStream(musica3);
    }

    static bool enter_consumido = false;

    if ((cancion_wed == 1 || cancion2 == 1 || cancion3 == 1) && IsKeyPressed(KEY_ENTER) && !mostrar_dif)
    {
        mostrar_dif = true;
        enter_consumido = true;
    }

    if (mostrar_dif)
    {
        Rectangle normal{1221, 396, 405, 127.5f};
        Rectangle oni{1215, 570, 405, 127.5f};

        Rectangle regresar{1166.5f, 745.5f, 195, 127.5f};

        DrawRectanglePro({1215, 390, 417, 139.5f}, Vector2{0, 0}, 12.0f, WHITE);

        DrawRectanglePro(normal, Vector2{0, 0}, 12.0f, (dificultad == 1) ? rojo : gris);
        DrawTextPro(miFuente, "NORMAL",
                    (Vector2){1290, 439.5f}, (Vector2){0, 0},
                    12.0f, 67.5f, 3.0f, BLACK);
        DrawTextPro(miFuente, "NORMAL",
                    (Vector2){1295.5f, 435}, (Vector2){0, 0},
                    12.0f, 67.5f, 3.0f, WHITE);

        DrawRectanglePro({1209, 564, 417, 139.5f}, Vector2{0, 0}, 12.0f, WHITE);

        DrawRectanglePro(oni, Vector2{0, 0}, 12.0f, (dificultad == 2) ? rojo : gris);
        DrawTextPro(miFuente, "ONI",
                    (Vector2){1347, 619.5f}, (Vector2){0, 0},
                    12.0f, 67.5f, 3.0f, BLACK);
        DrawTextPro(miFuente, "ONI",
                    (Vector2){1342.5f, 615}, (Vector2){0, 0},
                    12.0f, 67.5f, 3.0f, WHITE);

        DrawRectanglePro({1160.5f, 739.5f, 207, 139.5f}, Vector2{0, 0}, 12.0f, WHITE);

        DrawRectanglePro(regresar, Vector2{0, 0}, 12.0f, (dificultad == 3) ? rojo : gris);
        DrawTextPro(miFuente, "ATRAS",
                    (Vector2){1174.5f, 789.5f}, (Vector2){0, 0},
                    12.0f, 52.5f, 2.25f, BLACK);
        DrawTextPro(miFuente, "ATRAS",
                    (Vector2){1170, 785}, (Vector2){0, 0},
                    12.0f, 52.5f, 2.25f, WHITE);

        int cancion_seleccionada = 1;
        if (cancion_wed == 1)
            cancion_seleccionada = 1;
        else if (cancion2 == 1)
            cancion_seleccionada = 2;
        else if (cancion3 == 1)
            cancion_seleccionada = 3;

        records_menu(cancion_seleccionada, dificultad);

        if (!IsKeyDown(KEY_ENTER))
        {
            enter_consumido = false;
        }

        if (dificultad == 1 && (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)))
        {
            dificultad = 2;
        }
        else
        {
            if (dificultad == 2 && (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)))
            {
                dificultad = 3;
            }
            else
            {
                if (dificultad == 2 && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)))
                {
                    dificultad = 1;
                }
                else
                {
                    if (dificultad == 3 && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)))
                    {
                        dificultad = 2;
                    }
                }
            }
        }

        if (cancion_wed == 1 && dificultad == 1 && IsKeyPressed(KEY_ENTER) && !enter_consumido)
        {
            Pantalla_actual = CANCION1;
            mostrar_dif = false;
            dificultad = 1;
            StopMusicStream(musica1);
            return 1;
        }
        else
        {
            if (cancion_wed == 1 && dificultad == 2 && IsKeyPressed(KEY_ENTER) && !enter_consumido)
            {
                Pantalla_actual = CANCION1;
                mostrar_dif = false;
                dificultad = 1;
                StopMusicStream(musica1);
                return 2;
            }
            else
            {
                if (cancion_wed == 1 && dificultad == 3 && IsKeyPressed(KEY_ENTER) && !enter_consumido)
                {
                    mostrar_dif = false;
                    dificultad = 1;
                }
            }
        }

        if (cancion2 == 1 && dificultad == 1 && IsKeyPressed(KEY_ENTER) && !enter_consumido)
        {
            Pantalla_actual = CANCION2;
            mostrar_dif = false;
            dificultad = 1;
            StopMusicStream(musica1);
            return 1;
        }
        else
        {
            if (cancion2 == 1 && dificultad == 2 && IsKeyPressed(KEY_ENTER) && !enter_consumido)
            {
                Pantalla_actual = CANCION2;
                mostrar_dif = false;
                dificultad = 1;
                StopMusicStream(musica1);
                return 2;
            }
            else
            {
                if (cancion2 == 1 && dificultad == 3 && IsKeyPressed(KEY_ENTER) && !enter_consumido)
                {
                    mostrar_dif = false;
                    dificultad = 1;
                }
            }

            if (cancion3 == 1 && dificultad == 1 && IsKeyPressed(KEY_ENTER) && !enter_consumido)
            {
                Pantalla_actual = CANCION3;
                mostrar_dif = false;
                dificultad = 1;
                StopMusicStream(musica1);
                return 1;
            }
            else
            {
                if (cancion3 == 1 && dificultad == 2 && IsKeyPressed(KEY_ENTER) && !enter_consumido)
                {
                    Pantalla_actual = CANCION3;
                    mostrar_dif = false;
                    dificultad = 1;
                    StopMusicStream(musica1);
                    return 2;
                }
                else
                {
                    if (cancion3 == 1 && dificultad == 3 && IsKeyPressed(KEY_ENTER) && !enter_consumido)
                    {
                        mostrar_dif = false;
                        dificultad = 1;
                    }
                }
            }
        }
    }

    if (!mostrar_dif)
    {
        // BURBUJA DE DIALOGO CONTROLES
        DrawCircle(1755, 735, 12, WHITE);
        DrawCircle(1770, 720, 18, WHITE);
        DrawRectangleRounded((Rectangle){1480, 520, 380, 180}, 0.3f, 10, WHITE);

        DrawCircleLines(1755, 735, 12, BLACK);
        DrawCircleLines(1770, 720, 18, BLACK);
        DrawRectangleRoundedLines((Rectangle){1480, 520, 380, 180}, 0.3f, 10, BLACK);

        DrawTextEx(miFuente, "USA LAS FLECHAS", (Vector2){1550, 545}, 30, 1.5, BLACK);
        DrawTextEx(miFuente, "O W Y S PARA", (Vector2){1570, 580}, 30, 1.5, BLACK);
        DrawTextEx(miFuente, "MOVERTE!", (Vector2){1610, 615}, 30, 1.5, BLACK);
        DrawTextEx(miFuente, "ENTER PARA ELEGIR", (Vector2){1535, 655}, 28, 1.5, RED);
    }

    EndDrawing();
    return 0;
}

void cancion1(int dificultad)
{
    // VARIABLES DISEÑO
    static Texture2D fondo;
    // VARIABLES JUGABILIDAD
    static int cargado = 0;
    static Music musica = {0};
    static double tiempo_toque;
    static float tiempo_empezar = 3.0f;

    if (cargado == 0)
    {

        resultados_actuales.perfects = 0;
        resultados_actuales.goods = 0;
        resultados_actuales.misses = 0;
        resultados_actuales.max_combo = 0;
        resultados_actuales.score_final = 0;
    }

    if (Pantalla_actual != CANCION1 && cargado == 1)
    {
        // Liberar recursos
        if (musica.stream.buffer != NULL)
        {
            StopMusicStream(musica);
            UnloadMusicStream(musica);
            musica = (Music){0};
        }
        UnloadTexture(fondo);

        // Resetear estado
        cargado = 0;
        tiempo_empezar = 3.0f;
        tiempo_toque = 0.0;

        return;
    }

    if (stop == false)
    {
        if (cargado == 0)
        {
            tiempo_empezar -= GetFrameTime();

            if (tiempo_empezar <= 0.0f)
            {

                StopMusicStream(musica_inicio);

                if (musica.stream.buffer == NULL)
                {
                    musica = LoadMusicStream("assets/music/dominator.mp3");
                    musica.looping = false;
                }
                StopMusicStream(musica);
                SeekMusicStream(musica, 0);
                PlayMusicStream(musica);
                if (dificultad == 1)
                {
                    inicializar_notas();
                }
                else
                {
                    inicializar_notas_oni();
                }

                fondo = LoadTexture("assets/images/umi_fondo.jpeg");
                cargado = 1;
            }
        }
        else
        {
            UpdateMusicStream(musica);
        }

        // DIBUJAR CONTADOR
        if (cargado == 0)
        {
            // Calculamos el número visual (3, 2, 1)
            int empezar = (int)tiempo_empezar + 1;

            BeginDrawing();
            ClearBackground(BLACK);

            char texto_contador[10];
            if (empezar > 0)
            {
                sprintf(texto_contador, "%d", empezar);
                DrawText(texto_contador, 825, 450, 225, RED);
            }
            else
            {
                DrawText("GO!", 750, 450, 225, RED);
            }

            DrawText("CARGANDO...", 750, 750, 30, WHITE);

            EndDrawing();
            return;
        }

        float tiempo_musica = GetMusicTimePlayed(musica);

        if (IsKeyPressed(KEY_SPACE))
        {
            if (IsMusicStreamPlaying(musica))
                PauseMusicStream(musica);
            else
                ResumeMusicStream(musica);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(
            fondo,
            (Rectangle){0, 0, 1600, 872},
            (Rectangle){0, 0, 1920, 1080},
            (Vector2){0, 0},
            0.0f,
            WHITE);
        if (dificultad == 1)
            momento_golpe(tiempo_musica, &tiempo_toque, musica, 422);
        else
            momento_golpe(tiempo_musica, &tiempo_toque, musica, 633);

        dibujar_interfaz_completa(1, tiempo_musica, score, combo);

        if (!IsMusicStreamPlaying(musica) && stop == false)
        {
            cancion_actual = 1;
            dificultad_actual = dificultad;

            StopMusicStream(musica);
            score = 0;
            combo = 0;
            cargado = 0;

            Pantalla_actual = RESULTADOS;
            return;
        }
    }
    else
    {

        dibujarMenuPausa();
        if (Pantalla_actual != CANCION1)
        {
            if (musica.stream.buffer != NULL)
            {
                StopMusicStream(musica);
                UnloadMusicStream(musica);
                musica = (Music){0};
            }
            UnloadTexture(fondo);
            cargado = 0;
            tiempo_empezar = 3.0f;
            tiempo_toque = 0.0;
            return;
        }
    }

    pause(musica);
    EndDrawing();
}

void cancion2(int dificultad)
{
    static Texture2D fondo;
    static int cargado = 0;
    static Music musica = {0};
    static double tiempo_toque;
    static float tiempo_empezar = 3.0f;
    if (cargado == 0)
    {

        resultados_actuales.perfects = 0;
        resultados_actuales.goods = 0;
        resultados_actuales.misses = 0;
        resultados_actuales.max_combo = 0;
        resultados_actuales.score_final = 0;
    }

    if (Pantalla_actual != CANCION2 && cargado == 1)
    {
        // Liberar recursos
        if (musica.stream.buffer != NULL)
        {
            StopMusicStream(musica);
            UnloadMusicStream(musica);
            musica = (Music){0};
        }
        UnloadTexture(fondo);

        // Resetear estado
        cargado = 0;
        tiempo_empezar = 3.0f;
        tiempo_toque = 0.0;

        return; // Salir inmediatamente
    }

    if (stop == false)
    {
        if (cargado == 0)
        {
            tiempo_empezar -= GetFrameTime();

            if (tiempo_empezar <= 0.0f)
            {

                StopMusicStream(musica_inicio);

                if (musica.stream.buffer == NULL)
                {
                    musica = LoadMusicStream("assets/music/sun.ogg");
                    musica.looping = false;
                }
                StopMusicStream(musica);
                SeekMusicStream(musica, 0);
                PlayMusicStream(musica);
                if (dificultad == 1)
                {
                    notas_cancion2();
                }
                else
                {
                    notas_cancion2_oni();
                }
                fondo = LoadTexture("assets/images/fondo_canciones.png");
                cargado = 1;
            }
        }
        else
        {
            UpdateMusicStream(musica);
        }

        // DIBUJAR CONTADOR
        if (cargado == 0)
        {
            // Calculamos el número visual (3, 2, 1)
            int empezar = (int)tiempo_empezar + 1;

            BeginDrawing();
            ClearBackground(BLACK);

            char texto_contador[10];
            if (empezar > 0)
            {
                sprintf(texto_contador, "%d", empezar);
                DrawText(texto_contador, 825, 450, 225, RED);
            }
            else
            {
                DrawText("GO!", 750, 450, 225, RED);
            }

            DrawText("CARGANDO...", 750, 750, 30, WHITE);

            EndDrawing();
            return;
        }

        float tiempo_musica = GetMusicTimePlayed(musica);

        if (IsKeyPressed(KEY_SPACE))
        {
            if (IsMusicStreamPlaying(musica))
                PauseMusicStream(musica);
            else
                ResumeMusicStream(musica);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(
            fondo,
            (Rectangle){0, 0, 1024, 1024},
            (Rectangle){0, 0, 1920, 1080},
            (Vector2){0, 0},
            0.0f,
            WHITE);
        if (dificultad == 1)
            momento_golpe(tiempo_musica, &tiempo_toque, musica, 482);
        else
            momento_golpe(tiempo_musica, &tiempo_toque, musica, 962);

        dibujar_interfaz_completa(2, tiempo_musica, score, combo);

        if (!IsMusicStreamPlaying(musica) && stop == false)
        {
            cancion_actual = 2;
            dificultad_actual = dificultad;

            StopMusicStream(musica);
            score = 0;
            combo = 0;
            cargado = 0;

            Pantalla_actual = RESULTADOS;
            return;
        }
    }
    else
    {
        dibujarMenuPausa();
        if (Pantalla_actual != CANCION2)
        {
            if (musica.stream.buffer != NULL)
            {
                StopMusicStream(musica);
                UnloadMusicStream(musica);
                musica = (Music){0};
            }
            UnloadTexture(fondo);
            cargado = 0;
            tiempo_empezar = 3.0f;
            tiempo_toque = 0.0;
            return;
        }
    }
    pause(musica);
    EndDrawing();
}

void cancion3(int dificultad)
{
    static Texture2D fondo;
    static int cargado = 0;
    static Music musica = {0};
    static double tiempo_toque;
    static float tiempo_empezar = 3.0f;

    if (cargado == 0)
    {

        resultados_actuales.perfects = 0;
        resultados_actuales.goods = 0;
        resultados_actuales.misses = 0;
        resultados_actuales.max_combo = 0;
        resultados_actuales.score_final = 0;
    }

    if (Pantalla_actual != CANCION3 && cargado == 1)
    {
        // Liberar recursos
        if (musica.stream.buffer != NULL)
        {
            StopMusicStream(musica);
            UnloadMusicStream(musica);
            musica = (Music){0};
        }
        UnloadTexture(fondo);

        // Resetear estado
        cargado = 0;
        tiempo_empezar = 3.0f;
        tiempo_toque = 0.0;

        return; // Salir inmediatamente
    }

    if (stop == false)
    {
        if (cargado == 0)
        {
            tiempo_empezar -= GetFrameTime();

            if (tiempo_empezar <= 0.0f)
            {

                StopMusicStream(musica_inicio);

                if (musica.stream.buffer == NULL)
                {
                    musica = LoadMusicStream("assets/music/p3.ogg");
                    musica.looping = false;
                }
                StopMusicStream(musica);
                SeekMusicStream(musica, 0);
                PlayMusicStream(musica);
                if (dificultad == 1)
                {
                    notas_cancion3();
                }
                else
                {
                    notas_cancion3_oni();
                }
                fondo = LoadTexture("assets/images/fondo_canciones.png");
                cargado = 1;
            }
        }
        else
        {
            UpdateMusicStream(musica);
        }

        // DIBUJAR CONTADOR
        if (cargado == 0)
        {
            // Calculamos el numero visual (3, 2, 1)
            int empezar = (int)tiempo_empezar + 1;

            BeginDrawing();
            ClearBackground(BLACK);

            char texto_contador[10];
            if (empezar > 0)
            {
                sprintf(texto_contador, "%d", empezar);
                DrawText(texto_contador, 825, 450, 225, RED);
            }
            else
            {
                DrawText("GO!", 750, 450, 225, RED);
            }

            DrawText("CARGANDO...", 750, 750, 30, WHITE);

            EndDrawing();
            return;
        }

        float tiempo_musica = GetMusicTimePlayed(musica);

        if (IsKeyPressed(KEY_SPACE))
        {
            if (IsMusicStreamPlaying(musica))
                PauseMusicStream(musica);
            else
                ResumeMusicStream(musica);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(
            fondo,
            (Rectangle){0, 0, 1024, 1024},
            (Rectangle){0, 0, 1920, 1080},
            (Vector2){0, 0},
            0.0f,
            WHITE);

        if (dificultad == 1)
            momento_golpe(tiempo_musica, &tiempo_toque, musica, 355);
        else
            momento_golpe(tiempo_musica, &tiempo_toque, musica, 1055);

        dibujar_interfaz_completa(3, tiempo_musica, score, combo);

        if (!IsMusicStreamPlaying(musica) && stop == false)
        {
            cancion_actual = 3;
            dificultad_actual = dificultad;

            StopMusicStream(musica);
            score = 0;
            combo = 0;
            cargado = 0;

            Pantalla_actual = RESULTADOS;
            return;
        }
    }
    else
    {

        dibujarMenuPausa();
        if (Pantalla_actual != CANCION3)
        {
            if (musica.stream.buffer != NULL)
            {
                StopMusicStream(musica);
                UnloadMusicStream(musica);
                musica = (Music){0};
            }
            UnloadTexture(fondo);
            cargado = 0;
            tiempo_empezar = 3.0f;
            tiempo_toque = 0.0;
            return;
        }
    }
    pause(musica);
    EndDrawing();
}

void inicializar_notas()
{
    for (int i = 0; i < MAX_NOTAS; i++)
    {
        notas[i].hit = false;
        notas[i].time = 0.0;
        notas[i].tipe = 0;
    }
    double bpm = 143.0;

    double segundos_por_beat = 60.0 / bpm;

    float tiempo_inicio = 3.0;

    int patron_ritmico[] = {
        0, 1, 0, 1,
        0, 0, 1, 1,
        0, 1, 1, 0,
        1, 0, 1, 0,
        1, 1, 0, 1};
    int longitud_patron = 20;

    for (int i = 0; i < 422; i++)
    {
        notas[i].hit = false;

        notas[i].time = tiempo_inicio + (i * segundos_por_beat);

        notas[i].tipe = patron_ritmico[i % longitud_patron];
    }

    resultados_actuales.total_notas = 422;
}

void inicializar_notas_oni()
{
    for (int i = 0; i < MAX_NOTAS; i++)
    {
        notas[i].hit = false;
        notas[i].time = 0.0;
        notas[i].tipe = 0;
    }
    int i;
    double bpm = 143.0;

    double segundos_por_beat = 60.0 / bpm;
    double divisiones = 1.5;
    double oni = segundos_por_beat / divisiones;

    float tiempo_inicio = 3.0;

    int patron_ritmico[] = {
        0, 1, 0, 1,
        0, 0, 1, 1,
        0, 1, 1, 0,
        1, 0, 1, 0,
        1, 1, 0, 1};
    int longitud_patron = 20;

    for (i = 0; i < 633; i++)
    {
        notas[i].hit = false;

        notas[i].time = tiempo_inicio + (i * oni);

        notas[i].tipe = patron_ritmico[i % longitud_patron];
    }
    resultados_actuales.total_notas = 633;
    i = 0;
}

void notas_cancion2()
{
    for (int i = 0; i < MAX_NOTAS; i++)
    {
        notas[i].hit = false;
        notas[i].time = 0.0;
        notas[i].tipe = 0;
    }

    double bpm = 130.0;

    double segundos_por_beat = 60.0 / bpm;

    float tiempo_inicio = 3.0;

    int patron_ritmico[] = {
        0, 1, 0, 1,
        0, 0, 1, 1,
        0, 1, 1, 0,
        1, 0, 1, 0,
        1, 1, 0, 1};
    int longitud_patron = 20;

    for (int i = 0; i < 482; i++)
    {
        notas[i].hit = false;

        notas[i].time = tiempo_inicio + (i * segundos_por_beat);

        notas[i].tipe = patron_ritmico[i % longitud_patron];
    }
    resultados_actuales.total_notas = 482;
}

void notas_cancion2_oni()
{
    for (int i = 0; i < MAX_NOTAS; i++)
    {
        notas[i].hit = false;
        notas[i].time = 0.0;
        notas[i].tipe = 0;
    }

    double bpm = 130.0;

    double segundos_por_beat = 60.0 / bpm;
    double divisiones = 2;
    double oni = segundos_por_beat / divisiones;

    float tiempo_inicio = 3.0;

    int patron_ritmico[] = {
        0, 1, 0, 1,
        0, 0, 1, 1,
        0, 1, 1, 0,
        1, 0, 1, 0,
        1, 1, 0, 1};
    int longitud_patron = 20;

    for (int i = 0; i < 962; i++)
    {
        notas[i].hit = false;

        notas[i].time = tiempo_inicio + (i * oni);

        notas[i].tipe = patron_ritmico[i % longitud_patron];
    }
    resultados_actuales.total_notas = 962;
}

void notas_cancion3()
{
    for (int i = 0; i < MAX_NOTAS; i++)
    {
        notas[i].hit = false;
        notas[i].time = 0.0;
        notas[i].tipe = 0;
    }
    double bpm = 95.0;

    double segundos_por_beat = 60.0 / bpm;

    float tiempo_inicio = 3.0;

    int patron_ritmico[] = {
        0, 1, 0, 1,
        0, 0, 1, 1,
        0, 1, 1, 0,
        1, 0, 1, 0,
        1, 1, 0, 1};
    int longitud_patron = 20;

    for (int i = 0; i < 355; i++)
    {
        notas[i].hit = false;

        notas[i].time = tiempo_inicio + (i * segundos_por_beat);

        notas[i].tipe = patron_ritmico[i % longitud_patron];
    }
    resultados_actuales.total_notas = 355;
}

void notas_cancion3_oni()
{
    for (int i = 0; i < MAX_NOTAS; i++)
    {
        notas[i].hit = false;
        notas[i].time = 0.0;
        notas[i].tipe = 0;
    }
    double bpm = 95.0;

    double segundos_por_beat = 60.0 / bpm;
    double divisiones = 3;
    double oni = segundos_por_beat / divisiones;

    float tiempo_inicio = 3.0;

    int patron_ritmico[] = {
        0, 1, 0, 1,
        0, 0, 1, 1,
        0, 1, 1, 0,
        1, 0, 1, 0,
        1, 1, 0, 1};
    int longitud_patron = 20;

    for (int i = 0; i < 1055; i++)
    {
        notas[i].hit = false;

        notas[i].time = tiempo_inicio + (i * oni);

        notas[i].tipe = patron_ritmico[i % longitud_patron];
    }
    resultados_actuales.total_notas = 1055;
}

void momento_golpe(double tiempo, double *tiempo_golpe, Music musica, int max_notas)
{
    float preempt = 3.0f;
    static int j = 0;
    float inicio = 1845.0f;
    float zona_golpe = 150.0f;
    float carril_y = 450.0f;
    float carril_altura = 150.0f;
    float dt = GetFrameTime();
    static int ultima_nota_golpeada = -1;
    static float anim_timer = 0;

    DrawRectangle(zona_golpe, carril_y - carril_altura / 2,
                  inicio - zona_golpe, carril_altura,
                  (Color){20, 20, 20, 200});

    DrawLineEx((Vector2){zona_golpe, carril_y - carril_altura / 2},
               (Vector2){inicio, carril_y - carril_altura / 2},
               3.0f, WHITE);
    DrawLineEx((Vector2){zona_golpe, carril_y + carril_altura / 2},
               (Vector2){inicio, carril_y + carril_altura / 2},
               3.0f, WHITE);

    int golpe = detectar_golpes(tiempo_golpe, musica);
    double tiempo_musica = GetMusicTimePlayed(musica);

    for (int i = 0; i < max_notas; i++)
    {
        if (notas[i].hit)
            continue;

        double tiempo_antes_del_golpe = notas[i].time - tiempo_musica;

        if (tiempo_antes_del_golpe > -0.3f && tiempo_antes_del_golpe < preempt)
        {
            float progreso = tiempo_antes_del_golpe / preempt;
            float notax = zona_golpe + (inicio - zona_golpe) * progreso;

            if (notas[i].tipe == 0)
            {
                DrawCircle(notax, carril_y, 45, BLUE);
                DrawRing((Vector2){notax, carril_y}, 40, 45, 0, 360, 36, (Color){0, 0, 139, 255});
            }
            else
            {
                DrawCircle(notax, carril_y, 45, RED);
                DrawRing((Vector2){notax, carril_y}, 40, 45, 0, 360, 36, (Color){139, 0, 0, 255});
            }
        }

        if (golpe != -1)
        {
            double diff = fabs(notas[i].time - *tiempo_golpe);

            if (diff <= 0.12)
            {
                int puntuacion = 0;

                if (golpe == notas[i].tipe)
                {
                    if (diff <= 0.05)
                    {
                        puntuacion = 3;
                        resultados_actuales.perfects++;
                        mostrar_log("PERFECT!");
                    }
                    else
                    {
                        puntuacion = 2;
                        resultados_actuales.goods++;
                        mostrar_log("GOOD!");
                    }
                }
                else
                {
                    puntuacion = 1;
                    resultados_actuales.misses++;
                    mostrar_log("MISS!");
                }

                notas[i].hit = true;
                puntuaciones(puntuacion);

                anim_timer = 0;
                ultima_nota_golpeada = i;

                break;
            }
        }

        if (tiempo_antes_del_golpe < -0.3f)
        {
            notas[i].hit = true;
            resultados_actuales.misses++;
            puntuaciones(1);
        }
    }

    Color color_circulo = BLACK;

    if (anim_timer < 0.1 && ultima_nota_golpeada >= 0)
    {
        color_circulo = (notas[ultima_nota_golpeada].tipe == 1) ? RED : BLUE;
        anim_timer += dt;
    }

    DrawCircle(zona_golpe, carril_y, 80, color_circulo);
    DrawRing((Vector2){zona_golpe, carril_y}, 70, 80, 0, 360, 36, WHITE);
    DrawRing((Vector2){zona_golpe, carril_y}, 60, 70, 0, 360, 36, (Color){100, 100, 100, 255});
}

int detectar_golpes(double *tiempo_golpe, Music musica)
{
    // azules
    if (IsKeyPressed(KEY_S))
    {
        *tiempo_golpe = GetMusicTimePlayed(musica);
        return 0;
    }
    // rojas
    if (IsKeyPressed(KEY_D))
    {
        *tiempo_golpe = GetMusicTimePlayed(musica);
        return 1;
    }
    // azules
    if (IsKeyPressed(KEY_J))
    {
        *tiempo_golpe = GetMusicTimePlayed(musica);
        return 0;
    }

    // rojas
    if (IsKeyPressed(KEY_K))
    {
        *tiempo_golpe = GetMusicTimePlayed(musica);
        return 1;
    }

    return -1;
}

void puntuaciones(int puntos)
{
    if (puntos == 3)
    {
        score += 300;
        combo++;

        if (combo > resultados_actuales.max_combo)
            resultados_actuales.max_combo = combo;
    }
    else if (puntos == 2)
    {
        score += 100;
        combo++;

        if (combo > resultados_actuales.max_combo)
            resultados_actuales.max_combo = combo;
    }
    else
    {
        combo = 0;
        resultados_actuales.misses++;
    }

    resultados_actuales.score_final = score;
}

float easeOutBounce(float t)
{
    if (t < (1.0f / 2.75f))
    {
        return 7.5625f * t * t;
    }
    else if (t < (2.0f / 2.75f))
    {
        t -= (1.5f / 2.75f);
        return 7.5625f * t * t + 0.75f;
    }
    else if (t < (2.5f / 2.75f))
    {
        t -= (2.25f / 2.75f);
        return 7.5625f * t * t + 0.9375f;
    }
    else
    {
        t -= (2.625f / 2.75f);
        return 7.5625f * t * t + 0.984375f;
    }
}

void records_menu(int cancion, int dificultad)
{
    static Stats_Cancion *stats_canciones[3] = {
        &stats_cancion1,
        &stats_cancion2,
        &stats_cancion3};

    static float timer = 0.0f;
    static bool animando = true;
    static int cancion_anterior = -1;
    static int dificultad_anterior = -1;

    if (cancion != cancion_anterior || dificultad != dificultad_anterior)
    {
        timer = 0.0f;
        animando = true;
        cancion_anterior = cancion;
        dificultad_anterior = dificultad;
    }

    if (animando)
    {
        timer += GetFrameTime();
        if (timer >= 1.0f)
        {
            timer = 1.0f;
            animando = false;
        }
    }

    float progress = easeOutBounce(timer);

    float baseX = 1395.0f;
    float startX = 1920 + 525;
    float offsetX = startX + (baseX - startX) * progress;

    Stats_Cancion *stats_actual = stats_canciones[cancion - 1];
    int maxCombo, maxScore;

    if (dificultad == 1)
    {
        maxCombo = stats_actual->maxcombo_normal;
        maxScore = stats_actual->highscore_normal;
    }
    else
    {
        maxCombo = stats_actual->maxcombo_oni;
        maxScore = stats_actual->highscore_oni;
    }

    Color amarillo = (Color){255, 200, 0, 255};
    Color colorCombo = (maxCombo == 0) ? amarillo : BLACK;

    float fontSize = 37.5f;
    float spacing = 1.5f;

    // MAX COMBO
    DrawRectanglePro((Rectangle){offsetX, 45, 420, 68}, (Vector2){0, 0}, 7.5f, amarillo);
    DrawRectanglePro((Rectangle){offsetX, 45, 420, 22.5f}, (Vector2){0, 0}, 7.5f, BLACK);

    DrawTextPro(miFuente, "MAX COMBO:",
                (Vector2){offsetX + 19, 69.5f}, (Vector2){0, 0},
                7.5f, fontSize, spacing, BLACK); // +13 -> *1.5

    const char *comboTxt = TextFormat("%d", maxCombo);
    Vector2 sizeCombo = MeasureTextEx(miFuente, comboTxt, fontSize, spacing);

    DrawTextPro(miFuente, comboTxt,
                (Vector2){offsetX + 420 - sizeCombo.x - 22.5f, 120},
                (Vector2){0, 0}, 7.5f, fontSize, spacing, BLACK);

    // SCORE
    DrawRectanglePro((Rectangle){offsetX, 127.5f, 420, 68}, (Vector2){0, 0}, 7.5f, amarillo);
    DrawRectanglePro((Rectangle){offsetX, 127.5f, 420, 22.5f}, (Vector2){0, 0}, 7.5f, BLACK);

    DrawTextPro(miFuente, "SCORE:",
                (Vector2){offsetX + 19, 152}, (Vector2){0, 0},
                7.5f, fontSize, spacing, BLACK);

    const char *scoreTxt = TextFormat("%d", maxScore);
    Vector2 sizeScore = MeasureTextEx(miFuente, scoreTxt, fontSize, spacing);

    DrawTextPro(miFuente, scoreTxt,
                (Vector2){offsetX + 420 - sizeScore.x - 22.5f, 205},
                (Vector2){0, 0}, 7.5f, fontSize, spacing, BLACK);
}

void pause(Music musica)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        stop = !stop;
    }

    if (stop == true)
    {
        PauseMusicStream(musica);
    }
    else
    {
        ResumeMusicStream(musica);
    }
}

void dibujar_interfaz_completa(int cancion, float tiempo_musica, int score, int combo)
{
    static int cargado = 0;
    static Texture2D fondo;
    static Texture2D logo_cancion1;
    static Texture2D logo_cancion2;
    static Texture2D logo_cancion3;
    static Texture2D *spriteActual;
    static Texture2D *spriteActual2;
    static Texture2D sprite1;
    static Texture2D sprite2;
    static Texture2D sprite3;
    static Texture2D sprite5;
    static Texture2D sprite7;
    static Texture2D sprite8;
    static Texture2D sprite9;
    static Texture2D controles1, controles2;
    static float timer = 0;
    float duracion_sprite1 = 0.7f;
    float duracion_sprite2 = 0.7f;
    float duracion_sprite3 = 0.7f;
    float duracion_sprite7 = 0.7f;
    float duracion_sprite8 = 0.7f;
    float duracion_sprite9 = 0.7f;
    static float bounce_timer = 0;
    static int combo_anterior = 0;
    static int mostrar_racha_50 = 0;
    float duracion_bounce = 0.5f;

    if (cargado == 0)
    {
        sprite1 = LoadTexture("assets/images/sprite1_cancion.png");
        sprite2 = LoadTexture("assets/images/sprite2_cancion.png");
        sprite3 = LoadTexture("assets/images/sprite3_cancion.png");
        sprite5 = LoadTexture("assets/images/sprite_racha3.png");
        sprite7 = LoadTexture("assets/images/sprite7.png");
        sprite8 = LoadTexture("assets/images/sprite8.png");
        sprite9 = LoadTexture("assets/images/sprite9.png");
        logo_cancion1 = LoadTexture("assets/images/logo_cancion1.png");
        logo_cancion2 = LoadTexture("assets/images/logo_cancion2.png");
        logo_cancion3 = LoadTexture("assets/images/logo_cancion3.png");
        fondo = LoadTexture("assets/images/fondo_canciones.png");
        controles1 = LoadTexture("assets/images/controles1.png");
        controles2 = LoadTexture("assets/images/controles2.png");
        spriteActual = &sprite1;
        spriteActual2 = &sprite7;
        cargado = 1;
    }

    if (combo >= 50 && combo_anterior < 50)
    {
        mostrar_racha_50 = 1;
        bounce_timer = 0;
    }

    combo_anterior = combo;

    switch (cancion)
    {
    case 1:
    {
        DrawTextureEx(logo_cancion1, (Vector2){415, -100}, 0.0f, 0.49f, WHITE);
        break;
    }

    case 2:
    {
        DrawTextureEx(logo_cancion2, (Vector2){415, -100}, 0.0f, 0.49f, WHITE);
        break;
    }

    case 3:
    {
        DrawTextureEx(logo_cancion3, (Vector2){415, -100}, 0.0f, 0.49f, WHITE);
        break;
    }
    }

    if (log_timer > 0)
    {
        log_timer -= GetFrameTime();

        Color color = WHITE;
        if (strstr(ultimo_log, "PERFECT!"))
        {
            color = GREEN;
        }
        else if (strstr(ultimo_log, "GOOD!"))
        {
            color = YELLOW;
        }
        else if (strstr(ultimo_log, "MISS!"))
        {
            color = RED;
        }

        DrawTextPro(miFuente, ultimo_log, (Vector2){450, 600}, Vector2{0, 0}, 0.0f, 60, 1, color);
    }

    DrawRectangle(1084, 84, 250, 140, Fade(BLACK, 0.5f));
    DrawRectangle(1080, 80, 250, 140, (Color){25, 25, 25, 230});
    DrawRectangle(1080, 80, 250, 5, RED);
    DrawRectangleLines(1080, 80, 250, 140, RED);
    DrawTextEx(miFuente, "SCORE", (Vector2){1120, 100}, 40, 1, YELLOW);
    DrawTextEx(miFuente, TextFormat("%d", score), (Vector2){1150, 140}, 50, 1, RED);

    DrawRectangle(1384, 84, 250, 140, Fade(BLACK, 0.5f));
    DrawRectangle(1380, 80, 250, 140, (Color){25, 25, 25, 230});
    DrawRectangle(1380, 80, 250, 5, RED);
    DrawRectangleLines(1380, 80, 250, 140, RED);
    DrawTextEx(miFuente, "COMBO", (Vector2){1420, 100}, 40, 1, YELLOW);
    DrawTextEx(miFuente, TextFormat("%d", combo), (Vector2){1450, 140}, 50, 1, RED);

    DrawText(TextFormat("%.2f", tiempo_musica), 15, 30, 25, (Color){255, 255, 255, 150});

    float dt = GetFrameTime();
    timer += dt;

    if (spriteActual == &sprite1 && timer >= duracion_sprite1)
    {
        spriteActual = &sprite2;
        spriteActual2 = &sprite8;
        timer = 0;
    }

    if (spriteActual == &sprite2 && timer >= duracion_sprite2)
    {
        spriteActual = &sprite3;
        spriteActual2 = &sprite9;
        timer = 0;
    }

    if (spriteActual == &sprite3 && timer >= duracion_sprite3)
    {
        spriteActual = &sprite1;
        spriteActual2 = &sprite7;
        timer = 0;
    }

    if (spriteActual == &sprite1 && spriteActual2 == &sprite7)
    {
        DrawTextureEx(*spriteActual, (Vector2){110, 530}, 0.0f, 0.5f, WHITE);
        DrawTextureEx(*spriteActual2, (Vector2){1410, 510}, 0.0f, 0.45f, WHITE);
    }
    else
    {
        if (spriteActual == &sprite2 && spriteActual2 == &sprite8)
        {
            DrawTextureEx(*spriteActual, (Vector2){110, 530}, 0.0f, 0.5f, WHITE);
            DrawTextureEx(*spriteActual2, (Vector2){1410, 510}, 0.0f, 0.45f, WHITE);
        }
        else
        {
            if (spriteActual == &sprite3 && spriteActual2 == &sprite9)
            {
                DrawTextureEx(*spriteActual, (Vector2){110, 530}, 0.0f, 0.52f, WHITE);
                DrawTextureEx(*spriteActual2, (Vector2){1410, 510}, 0.0f, 0.45f, WHITE);
            }
        }
    }

    if (mostrar_racha_50)
    {
        bounce_timer += dt;

        float escala_bounce = 1.0f + sinf(bounce_timer * 5.0f) * 0.15f;

        if (mostrar_racha_50 && combo >= 50)
        {
            DrawTextureEx(sprite5, (Vector2){650, 530}, 0.0f, 0.7f * escala_bounce, WHITE);
        }
    }
    if (combo < 50)
    {
        mostrar_racha_50 = 0;
    }

    if (!mostrar_racha_50)
    {
        DrawCircle(1420, 740, 12, WHITE);
        DrawCircle(1395, 735, 18, WHITE);
        DrawRectangleRounded((Rectangle){1050, 570, 380, 130}, 0.3f, 10, WHITE);

        DrawCircleLines(1420, 740, 12, BLACK);
        DrawCircleLines(1395, 735, 18, BLACK);
        DrawRectangleRoundedLines((Rectangle){1050, 570, 380, 130}, 0.3f, 10, BLACK);

        DrawTextEx(miFuente, "USA ESTAS TECLAS!", (Vector2){1090, 590}, 28, 1.5, BLACK);

        // CONTROLES
        DrawTextureEx(controles1, (Vector2){1080, 610}, 0.0f, 0.15f, WHITE);
        DrawTextureEx(controles2, (Vector2){1245, 610}, 0.0f, 0.15f, WHITE);
    }
}

void mostrar_log(const char *mensaje)
{
    if (log_timer <= 0.0f)
    {
        strcpy(ultimo_log, mensaje);
        log_timer = 0.3f;
    }
}

void pantalla_resultados(int cancion, int dificultad)
{
    static int cargado = 0;
    static Texture2D fondo;
    static float timer_animacion = 0;
    float accuracy = 0;
    float peso_perfect;
    float peso_good;
    int total_jugadas = resultados_actuales.perfects + resultados_actuales.goods + resultados_actuales.misses;

    if (cargado == 0)
    {
        fondo = LoadTexture("assets/images/fondo_canciones.png");
        timer_animacion = 0;
        cargado = 1;
        accuracy = 0;
    }

    timer_animacion += GetFrameTime();

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexturePro(
        fondo,
        (Rectangle){0, 0, 1024, 1024},
        (Rectangle){0, 0, 1920, 1080},
        (Vector2){0, 0}, 0.0f, Fade(WHITE, 0.3f));

    Color rojo = (Color){230, 0, 18, 255};
    Color amarillo = (Color){255, 200, 0, 255};
    Color verde = (Color){0, 255, 100, 255};

    if (resultados_actuales.total_notas > 0)
    {
        int notas_acertadas = resultados_actuales.perfects + resultados_actuales.goods;
        accuracy = ((float)notas_acertadas / (float)total_jugadas) * 100.0f;
    }

    char ranking = 'F';
    Color color_ranking = GRAY;
    if (accuracy >= 95.0f)
    {
        ranking = 'S';
        color_ranking = (Color){255, 215, 0, 255};
    }
    else if (accuracy >= 90.0f)
    {
        ranking = 'A';
        color_ranking = verde;
    }
    else if (accuracy >= 80.0f)
    {
        ranking = 'B';
        color_ranking = (Color){100, 200, 255, 255};
    }
    else if (accuracy >= 70.0f)
    {
        ranking = 'C';
        color_ranking = (Color){255, 255, 100, 255};
    }
    else if (accuracy >= 60.0f)
    {
        ranking = 'D';
        color_ranking = (Color){255, 150, 0, 255};
    }

    bool full_combo = (resultados_actuales.misses == 0);
    bool perfect_play = (resultados_actuales.goods == 0 && resultados_actuales.misses == 0);

    DrawTextEx(miFuente, "RESULTADOS", (Vector2){650, 50}, 80, 3, rojo);

    float escala = 1.0f + sinf(timer_animacion * 2.0f) * 0.1f;

    // DIBUJA EL RANKING
    DrawTextEx(miFuente, TextFormat("%c", ranking),
               (Vector2){200, 300}, 250 * escala, 5, color_ranking);
    DrawTextEx(miFuente, "RANK", (Vector2){220, 550}, 50, 2, WHITE);

    // DIBUJA EL ACCURACY
    DrawTextEx(miFuente, TextFormat("%.1f%%", accuracy),
               (Vector2){200, 650}, 60, 2, amarillo);
    DrawTextEx(miFuente, "ACCURACY", (Vector2){200, 720}, 35, 2, WHITE);

    float px = 700, py = 200, sy = 100;

    DrawRectangle(px - 10, py - 10, 1000, 80, Fade(BLACK, 0.7f));
    DrawRectangleLines(px - 10, py - 10, 1000, 80, rojo);
    DrawTextEx(miFuente, "SCORE:", (Vector2){px, py + 10}, 50, 2, amarillo);
    DrawTextEx(miFuente, TextFormat("%d", resultados_actuales.score_final),
               (Vector2){px + 600, py + 10}, 50, 2, WHITE);

    py += sy;
    DrawRectangle(px - 10, py - 10, 1000, 80, Fade(BLACK, 0.7f));
    DrawRectangleLines(px - 10, py - 10, 1000, 80, rojo);
    DrawTextEx(miFuente, "MAX COMBO:", (Vector2){px, py + 10}, 50, 2, amarillo);
    DrawTextEx(miFuente, TextFormat("%d", resultados_actuales.max_combo),
               (Vector2){px + 600, py + 10}, 50, 2, WHITE);

    py += sy;
    DrawRectangle(px - 10, py - 10, 1000, 80, Fade(BLACK, 0.7f));
    DrawRectangleLines(px - 10, py - 10, 1000, 80, verde);
    DrawTextEx(miFuente, "PERFECT:", (Vector2){px, py + 10}, 50, 2, verde);
    DrawTextEx(miFuente, TextFormat("%d", resultados_actuales.perfects),
               (Vector2){px + 600, py + 10}, 50, 2, WHITE);

    py += sy;
    DrawRectangle(px - 10, py - 10, 1000, 80, Fade(BLACK, 0.7f));
    DrawRectangleLines(px - 10, py - 10, 1000, 80, YELLOW);
    DrawTextEx(miFuente, "GOOD:", (Vector2){px, py + 10}, 50, 2, YELLOW);
    DrawTextEx(miFuente, TextFormat("%d", resultados_actuales.goods),
               (Vector2){px + 600, py + 10}, 50, 2, WHITE);

    py += sy;
    DrawRectangle(px - 10, py - 10, 1000, 80, Fade(BLACK, 0.7f));
    DrawRectangleLines(px - 10, py - 10, 1000, 80, RED);
    DrawTextEx(miFuente, "MISS:", (Vector2){px, py + 10}, 50, 2, RED);
    DrawTextEx(miFuente, TextFormat("%d", resultados_actuales.misses),
               (Vector2){px + 600, py + 10}, 50, 2, WHITE);

    if (perfect_play)
    {
        float b = 0.5f + sinf(timer_animacion * 5.0f) * 0.5f;
        DrawTextEx(miFuente, "*** PERFECT PLAY ***",
                   (Vector2){600, 850}, 60, 2,
                   Fade((Color){255, 215, 0, 255}, b));
    }
    else if (full_combo)
    {
        float b = 0.5f + sinf(timer_animacion * 5.0f) * 0.5f;
        DrawTextEx(miFuente, "*** FULL COMBO ***",
                   (Vector2){650, 850}, 60, 2, Fade(verde, b));
    }

    // CUANDO HAY NUEVO RECORD
    Stats_Cancion *stats = NULL;
    if (cancion == 1)
        stats = &stats_cancion1;
    else if (cancion == 2)
        stats = &stats_cancion2;
    else if (cancion == 3)
        stats = &stats_cancion3;

    if (stats)
    {
        int *hs = (dificultad == 1) ? &stats->highscore_normal : &stats->highscore_oni;
        int *mc = (dificultad == 1) ? &stats->maxcombo_normal : &stats->maxcombo_oni;

        bool nuevo = false;
        if (resultados_actuales.score_final > *hs)
        {
            *hs = resultados_actuales.score_final;
            nuevo = true;
        }
        if (resultados_actuales.max_combo > *mc)
        {
            *mc = resultados_actuales.max_combo;
            nuevo = true;
        }

        if (nuevo)
        {
            float e = 1.0f + sinf(timer_animacion * 4.0f) * 0.15f;
            DrawTextEx(miFuente, "NEW RECORD!", (Vector2){750, 920}, 50 * e, 2, rojo);
        }
    }

    DrawTextEx(miFuente, "PRESIONA ENTER PARA CONTINUAR",
               (Vector2){550, 900}, 40, 2,
               Fade(WHITE, 0.5f + sinf(timer_animacion * 3.0f) * 0.5f));

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE))
    {
        resultados_actuales = (Resultados){0, 0, 0, 0, 0, 0};
        cargado = 0;
        timer_animacion = 0;
        Pantalla_actual = MENU_CANCIONES;
    }

    EndDrawing();
}

void dibujarMenuPausa()
{

    DrawRectangle(765, 395, 410, 210, WHITE);
    DrawRectangle(768, 400, 400, 200, DARKGRAY);

    DrawTextPro(miFuente, "REANUDAR", (Vector2){800, 450}, (Vector2){0, 0}, 0, 40, 1, opcionMenuPausa == 0 ? RED : WHITE);
    DrawTextPro(miFuente, "SALIR", (Vector2){800, 520}, (Vector2){0, 0}, 0, 40, 1, opcionMenuPausa == 1 ? RED : WHITE);

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN))
        opcionMenuPausa = 1 - opcionMenuPausa;
    else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_S))
        opcionMenuPausa = 1 - opcionMenuPausa;

    if (IsKeyPressed(KEY_ENTER))
    {
        if (opcionMenuPausa == 0)
        {
            stop = false;
        }
        else if (opcionMenuPausa == 1)
        {
            Pantalla_actual = MENU_CANCIONES;
            while (GetKeyPressed() != 0)
            {
            }
        }
    }
}

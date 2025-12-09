typedef struct
{
  int highscore_normal;
  int highscore_oni;
  int maxcombo_normal;
  int maxcombo_oni;
} Stats_Cancion;

typedef struct {
    int perfects;
    int goods;
    int misses;
    int max_combo;
    int score_final;
    int total_notas;
} Resultados;

typedef enum
{
  INICIO,
  MENU_CANCIONES,
  CANCION1,
  CANCION2,
  CANCION3,
  RESULTADOS
} Pantalla;

typedef struct _notes
{
  bool hit;
  int tipe;
  float time;

} Ts_notas;

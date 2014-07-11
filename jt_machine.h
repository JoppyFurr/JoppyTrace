typedef enum
{
    JT_STATE_INIT,
    JT_STATE_ABORT,
    JT_STATE_READY,
    JT_STATE_RUNNING,
    JT_STATE_QUIT

} jt_state_t;

typedef enum
{
    JT_MODE_INIT,
    JT_MODE_STILL,
    JT_MODE_ANIMATION
} jt_mode_t;

typedef enum /* TODO: This belongs in jt_types.h */
{
    JT_FALSE = 0,
    JT_TRUE
} jt_bool_t;

typedef struct jt_machine_s
{
    jt_state_t      state;
    jt_mode_t       mode;
    jt_bool_t       debug;
    uint32_t        width;
    uint32_t        height;
    uint8_t         pixels[640 * 480 * sizeof (uint32_t)];

    /* Threading */
    pthread_t      *thread;
    uint32_t        thread_count;
    pthread_mutex_t work_mutex;
    sem_t          *wake_sem;
    uint32_t        work_next_chunk;
    uint32_t        work_total_chunks;
    uint32_t        work_complete_chunks;
    void            (*work_do_chunk) (uint32_t chunk);

    /* SDL */
    SDL_Window     *window;
    SDL_Renderer   *renderer;
    SDL_Texture    *texture;
} jt_machine_t;


extern void (*yield)(void);
extern void schedule();
extern void schedule_init(void);
extern void load_one_task(void (*fn)(void));
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define TASKS_MAX 30
#define MACHINES_MAX 20

int n, k;
int task[TASKS_MAX];

int min;
unsigned long assignedTasks[MACHINES_MAX];

static int id[TASKS_MAX];
static int total[MACHINES_MAX];
static int curr;

static void findMinTime(int i, int j)
{
    if (i == n) {
        min = curr;
        return;
    }

    if (j < k) {
        total[j] = task[i];
        findMinTime(i + 1, j + 1);
    }

    for (int l = 0; l < j; ++l) {
        if (total[l] + task[i] < min) {
            total[l] += task[i];
            int save = curr;
            if (total[l] > curr)
                curr = total[l];

			findMinTime(i + 1, j);

            total[l] -= task[i];
            curr = save;

            if (curr == min)
                break;
        }
    }
}

static bool findStrategy(int i, int j)
{
    if (i == n)
        return true;

    if (j < k) {
        total[j] = task[i];

        if (findStrategy(i + 1, j + 1)) {
            assignedTasks[j] |= 1 << id[i];
            return true;
        }
    }

    for (int l = 0; l < j; ++l) {
        if (total[l] + task[i] <= min) {
            total[l] += task[i];

            if (findStrategy(i + 1, j)) {
                assignedTasks[l] |= 1 << id[i];
                return true;
            }

            total[l] -= task[i];
        }
    }

    return false;
}

inline static void sortTask(void)
{
    for (int i = 0; i < n; ++i)
        id[i] = i;

    for (int i = 1; i < n; ++i) {
        int t = task[i], o = id[i];

        int j = i;
        for (; task[j - 1] < t && j > 0; --j) {
            task[j] = task[j - 1];
            id[j] = id[j - 1];
        }

        task[j] = t;
        id[j] = o;
    }
}

void schedule(void)
{
    sortTask();

    min = 0;
    for (int i = 0; i < n; i += k)
        min += task[i];

    total[0] = task[0];
    curr = task[0];
    findMinTime(1, 1);

    memset(assignedTasks, 0, sizeof(assignedTasks));
    total[0] = task[0];
    assignedTasks[0] = 1 << id[0];
    findStrategy(1, 1);
}

void test(int num)
{
    printf("Running test %d\n", num);

    char filename[20];
    snprintf(filename, sizeof(filename), "test%d.txt", num);
    FILE *fp = fopen(filename, "r");

    fscanf(fp, "%d%d", &n, &k);
    for (int i = 0; i < n; ++i) {
        fscanf(fp, "%d", &task[i]);
    }

    clock_t start = clock();
    schedule();
    clock_t end = clock();

    double diff = (double) (end - start) / CLOCKS_PER_SEC;
    printf("Algorithm completed in %.6lf seconds, solution:\n", diff);

    printf("Min time required: %d\n", min);
    printf("A strategy:\n");
    for (int i = 0; i < k; ++i) {
        printf("  Machine %d: tasks = {", i + 1);
        for (int j = 0; j < n; ++j) {
            if (assignedTasks[i] & (1 << j)) {
                printf("%d, ", j + 1);
            }
        }
        printf("}\n");
    }
    printf("\n");

    fclose(fp);
}

int main(void)
{
    test(1);
    test(2);
    test(3);
    return 0;
}

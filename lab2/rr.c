#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef uint32_t u32;
typedef int32_t i32;

struct process
{
  u32 pid;
  u32 arrival_time;
  u32 burst_time;

  TAILQ_ENTRY(process)
  pointers;

  /* Additional fields here */
  bool runned;
  u32 response_time;
  u32 completion_time;
  u32 remaining_time;
  /* End of "Additional fields here" */
};

TAILQ_HEAD(process_list, process);

u32 next_int(const char **data, const char *data_end)
{
  u32 current = 0;
  bool started = false;
  while (*data != data_end)
  {
    char c = **data;

    if (c < 0x30 || c > 0x39)
    {
      if (started)
      {
        return current;
      }
    }
    else
    {
      if (!started)
      {
        current = (c - 0x30);
        started = true;
      }
      else
      {
        current *= 10;
        current += (c - 0x30);
      }
    }

    ++(*data);
  }

  printf("Reached end of file while looking for another integer\n");
  exit(EINVAL);
}

u32 next_int_from_c_str(const char *data)
{
  char c;
  u32 i = 0;
  u32 current = 0;
  bool started = false;
  while ((c = data[i++]))
  {
    if (c < 0x30 || c > 0x39)
    {
      exit(EINVAL);
    }
    if (!started)
    {
      current = (c - 0x30);
      started = true;
    }
    else
    {
      current *= 10;
      current += (c - 0x30);
    }
  }
  return current;
}

void init_processes(const char *path,
                    struct process **process_data,
                    u32 *process_size)
{
  int fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    int err = errno;
    perror("open");
    exit(err);
  }

  struct stat st;
  if (fstat(fd, &st) == -1)
  {
    int err = errno;
    perror("stat");
    exit(err);
  }

  u32 size = st.st_size;
  const char *data_start = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data_start == MAP_FAILED)
  {
    int err = errno;
    perror("mmap");
    exit(err);
  }

  const char *data_end = data_start + size;
  const char *data = data_start;

  *process_size = next_int(&data, data_end);

  *process_data = calloc(sizeof(struct process), *process_size);
  if (*process_data == NULL)
  {
    int err = errno;
    perror("calloc");
    exit(err);
  }

  for (u32 i = 0; i < *process_size; ++i)
  {
    (*process_data)[i].pid = next_int(&data, data_end);
    (*process_data)[i].arrival_time = next_int(&data, data_end);
    (*process_data)[i].burst_time = next_int(&data, data_end);
    (*process_data)[i].runned = false;
    (*process_data)[i].response_time = 0;
    (*process_data)[i].completion_time = 0;
    (*process_data)[i].remaining_time = (*process_data)[i].burst_time;
  }

  munmap((void *)data, size);
  close(fd);
}
int compare_processes(const void *a, const void *b)
{
  const struct process *pa = a;
  const struct process *pb = b;

  if (pa->arrival_time < pb->arrival_time)
    return -1;
  if (pa->arrival_time > pb->arrival_time)
    return 1;
  if (pa->pid < pb->pid)
    return -1;
  if (pa->pid > pb->pid)
    return 1;

  return 0;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    return EINVAL;
  }
  struct process *data;
  u32 size;
  init_processes(argv[1], &data, &size);

  u32 quantum_length = next_int_from_c_str(argv[2]);

  struct process_list list;
  TAILQ_INIT(&list);

  u32 total_waiting_time = 0;
  u32 total_response_time = 0;

  /* Your code here */
  int finished_count = 0;
  int current_time = 0;
  int queued_index = 0;
  qsort(data, size, sizeof(struct process), compare_processes);
  while (finished_count < size)
  {
    if (TAILQ_EMPTY(&list))
    {
      current_time = data[queued_index].arrival_time;
      TAILQ_INSERT_TAIL(&list, &data[queued_index], pointers);
      queued_index++;
    }
    struct process *cur_process = TAILQ_FIRST(&list);
    TAILQ_REMOVE(&list, TAILQ_FIRST(&list), pointers);
    if (cur_process->runned == false)
    {
      cur_process->response_time = current_time - cur_process->arrival_time;
      total_response_time += cur_process->response_time;
      cur_process->runned = true;
    }
    if (cur_process->remaining_time <= quantum_length)
    {
      current_time += cur_process->remaining_time;
      cur_process->completion_time = current_time;
      int waiting_time= current_time - cur_process->arrival_time - cur_process->burst_time;
      total_waiting_time += waiting_time;
      finished_count++;
    }
    else
    {
      current_time += quantum_length;
      cur_process->remaining_time -= quantum_length;
      for(int i=queued_index;data[i].arrival_time<=current_time&&i<size;i++){
        TAILQ_INSERT_TAIL(&list, &data[i], pointers);
        queued_index++;
      }
      TAILQ_INSERT_TAIL(&list, cur_process, pointers);
    }
  }

  /* End of "Your code here" */
  printf("Average waiting time: %.2f\n", (float)total_waiting_time / (float)size);
  printf("Average response time: %.2f\n", (float)total_response_time / (float)size);

  free(data);
  return 0;
}

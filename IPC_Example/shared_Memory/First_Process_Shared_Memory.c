#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define KEY_NUM 1234
#define MEM_SIZE 4096

int shmid;
static int SharedMemoryCreate();
static int SharedMemoryWrite(char *shareddata, int size);
static int SharedMemoryRead(char *sMemory);
static int SharedMemoryFree(void);

int main(int argc, char *argv[])
{
    char buffer[MEM_SIZE] = {
        1,
    };

    SharedMemoryCreate();
    sleep(5);

    SharedMemoryWrite(buffer, sizeof(buffer));
    return 0;
}

static int SharedMemoryCreate()
{
    if ((shmid = shmget((key_t)KEY_NUM, MEM_SIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1) // shmget() : 커널에 공유 메모리 공간을 요청하기 위해 사용하는 시스템 호출 함수
    {                                                                                  // key 값은 고유의 공유 메모리임을 나타낸다.
        printf("There was shared memory.");

        shmid = shmget((key_t)KEY_NUM, MEM_SIZE, IPC_CREAT | 0666);

        if (shmid == -1)
        {
            perror("Shared memory create fail");
            return 1;
        }
        else
        {
            SharedMemoryFree();
            shmid = shmget((key_t)KEY_NUM, MEM_SIZE, IPC_CREAT | 0666);

            if (shmid == -1)
            {
                perror("Shared memory create fail");
                return 1;
            }
        }
    }

    return 0;
}

static int SharedMemoryWrite(char *shareddata, int size)
{
    void *shmaddr;
    if (size > MEM_SIZE)
    {
        printf("Shared memory size over");
        return 1;
    }

    if ((shmaddr = shmat(shmid, NULL, 0)) == (void *)-1) // shmat() : 공유 메모리 공간을 생성한 이후, 공유 메모리에 접근할 수 있는 int 형의 식별자를 얻는다.
    {                                                    // 공유 메모리를 사용하기 위해 얻은 식별자를 이용하여 현재 프로세스가 공유 메모리에 접근할 수 있도록 연결하는 작업
        perror("Shmat failed");
        return 1;
    }

    memcpy((char *)shmaddr, shareddata, size);

    if (shmdt(shmaddr) == -1)   // shmdt() : 프로세스가 더이상 공유 메모리를 사용하지 않을 경우 프로세스와 공유 메모리를 분리시키는 작업
    {                           // 해당 시스템 호출 함수는 현재 프로세스와 공유 메모리를 분리시킬 뿐, 공유 메모리의 공간을 삭제하지 않는다.
        perror("Shmdt failed"); // shmdt 가 성공적으로 수행되면 커널은 shmid_ds 의 내용을 갱신, 즉 shm_dtime, shm_lpid, shm_nattch 등의 내용을 갱신하는데,
        return 1;               // shm_dtime은 가장 최근에 dettach된 시간, shm_lpid는 호출한 프로세스의 PID,
    }                           // shm_nattch는 현재 공유 메모리를 사용하는프로세스의 수를 돌려준다.
    return 0;
}

static int SharedMemoryRead(char *sMemory)
{
    void *shmaddr;
    char mess[MEM_SIZE] = {0};

    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void *)-1)
    {
        perror("Shmat failed");
        return 1;
    }

    memcpy(sMemory, (char *)shmaddr, sizeof(mess));

    if (shmdt(shmaddr) == -1)
    {
        perror("Shmdt failed");
        return 1;
    }
    return 0;
}

static int SharedMemoryFree(void)
{
    if (shmctl(shmid, IPC_RMID, 0) == -1) // shmctl() : 공유 메모리를 제어하기 위해 사용
    {                                     // shmid_ds 구조체를 직접 제어함으로써, 해당 공유 메모리에 대한 소유자, 그룹 등의 허가권을 변경하거나, 공유 메모리 삭제.
        perror("Shmctl failed");          // 공유 메모리의 잠금을 설정하거나 해제하는 작업
        return 1;
    }

    printf("Shared memory end");
    return 0;
}
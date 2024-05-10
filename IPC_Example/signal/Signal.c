#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void _signal(int sig);
int main(void)
{
    (void)signal(SIGINT, _signal);
    /*  이제부터 현제 프로세스에게
     *  SIGINT에 해당하는 신호를 받으면,
     *  _signal이란 함수를 호출하게 됨
     */
    while (1)
    {
        printf("give me a signal\n");
        sleep(1);
    }
    exit(0);
}
void _signal(int sig)
{
    printf("You give a signal to me, thanks\n");
    (void)signal(SIGINT, SIG_DFL);
    // SIGINT 신호를 받아왔을때, 간략한 문장을 출력후
    // SIG_DEL (기본 행동 수행)으로 메인함수가
    // 하던일을 계속 수행하도록 한다.
    // 이후에 한번더 SIGINT를 날려주면 프로세스가 종료됨
}
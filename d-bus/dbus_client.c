#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <systemd/sd-bus.h>

// 알림 메시지 수신 시 호출되는 함수
int onNotification(
    sd_bus_message *m,
    void *userdata,
    sd_bus_error *ret_error
) {
    (void)userdata;
    (void)ret_error;

    const char *message;
    int r;

    r = sd_bus_message_read(m, "s", &message);

    if (r < 0)
        return r;

    printf("[Notification] %s\n", message);
    fflush(stdout);

    return 0;
}

int main(void) {
    sd_bus *bus = NULL;
    sd_bus_slot *slot = NULL;
    int r;

    // D-Bus 세션 버스 연결
    r = sd_bus_default_user(&bus);

    if (r < 0) {
        fprintf(stderr, "D-Bus connection failed: %s\n", strerror(-r));
        return 1;
    }

    // 알림 시그널 수신 등록
    r = sd_bus_add_match(
        bus,
        &slot,
        "type='signal',"
        "interface='com.example.Notification',"
        "member='Notify',"
        "path='/com/example/Notification'",
        onNotification,
        NULL
    );

    if (r < 0) {
        fprintf(stderr, "Match failed: %s\n", strerror(-r));
        sd_bus_unref(bus);
        return 1;
    }

    printf("===== D-Bus Notification Client =====\n");
    printf("Waiting for notifications...\n\n");

    // 메시지 수신 대기
    while (1) {
        r = sd_bus_process(bus, NULL);

        if (r < 0) {
            fprintf(stderr, "Process failed: %s\n", strerror(-r));
            break;
        }

        if (r > 0)
            continue;

        r = sd_bus_wait(bus, UINT64_MAX);

        if (r < 0) {
            fprintf(stderr, "Wait failed: %s\n", strerror(-r));
            break;
        }
    }

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);

    return 0;
}
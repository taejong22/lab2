#include <stdio.h>
#include <string.h>
#include <systemd/sd-bus.h>

#define SERVICE_NAME "com.example.Notification"
#define OBJECT_PATH "/com/example/Notification"
#define INTERFACE_NAME "com.example.Notification"

int main(void) {
    sd_bus *bus = NULL;
    char message[256];
    int r;

    // D-Bus 세션 버스 연결
    r = sd_bus_default_user(&bus);

    if (r < 0) {
        fprintf(stderr, "D-Bus connection failed: %s\n", strerror(-r));
        return 1;
    }

    // 서비스 이름 등록
    r = sd_bus_request_name(bus, SERVICE_NAME, 0);

    if (r < 0) {
        fprintf(stderr, "Request name failed: %s\n", strerror(-r));
        sd_bus_unref(bus);
        return 1;
    }

    printf("===== D-Bus Notification Server =====\n");
    printf("Type 'exit' to quit.\n\n");

    while (1) {
        printf("Message: ");
        fflush(stdout);

        if (fgets(message, sizeof(message), stdin) == NULL)
            break;

        message[strcspn(message, "\n")] = '\0';

        if (strcmp(message, "exit") == 0)
            break;

        // D-Bus 시그널 전송
        r = sd_bus_emit_signal(
            bus,
            OBJECT_PATH,
            INTERFACE_NAME,
            "Notify",
            "s",
            message
        );

        if (r < 0) {
            fprintf(stderr, "Send failed: %s\n", strerror(-r));
            break;
        }

        // 전송 대기 중인 메시지 처리
        r = sd_bus_flush(bus);

        if (r < 0) {
            fprintf(stderr, "Flush failed: %s\n", strerror(-r));
            break;
        }

        printf("Notification sent.\n\n");
    }

    sd_bus_unref(bus);

    printf("Server terminated.\n");

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define AI 17
#define SPADE "��"
#define CLUB "��"
#define DIA "��"
#define HEART "��"

// enum
enum shape { spade, club, dia, heart };
enum num { ace = 1, jack = 11, queen = 12, king = 13 };

// ī�� ����ü
struct Card {
    int number;
    char shape;
};

// ��� ī��
struct Card card_all[52];
int next;

// ����
struct Player {
    int cash;
    int score;
    int bust;
    struct Card card_player[21];
    int initial_cash;
};

struct Player player[2]; // ������ �÷��̾� 1�� ���

// ī�� ���� �Լ�
void shuffle()
{
    srand(time(NULL));
    for (int i = 0; i < 52; i++)
    {
        int index = rand() % 52;
        struct Card temp = card_all[index];
        card_all[index] = card_all[i];
        card_all[i] = temp;
    }
}

// ī�� ä��� �Լ�
void filldeck()
{
    int i = 0;
    // 4���� ���� 13���� ���ڸ� ¦��� ī�� ����
    for (int j = 0; j < 4; j++)
    {
        for (int k = 1; k < 14; k++)
        {
            card_all[i].number = k;
            card_all[i].shape = j;
            i++;
        }
    }
}

// ���� �� �տ��� ���� ���� �����ִ� �Լ�
struct Card deal(int next)
{
    return card_all[next];
}

// ����ڰ� ���� ī�带 ���ڿ��� �ƴ� ī�� ������� ���
void printcard(int p, int turns)
{
    player[p].score = 0; // Initialize the player's score

    if (p == 0)
    {
        printf("\n<<DEALER>>\n");
    }
    else
    {
        printf("\n<<PLAYER%d>>\n", p);
    }

    for (int i = 0; i <= turns; i++)
    {
        printf("------- "); // ī�� ������ ���� ���
    }

    printf("\n");

    for (int i = 0; i <= turns; i++)
    {
        printf("|%s    |", (player[p].card_player[i].shape == spade) ? SPADE : (player[p].card_player[i].shape == club) ? CLUB : (player[p].card_player[i].shape == dia) ? DIA : HEART);
    }

    printf("\n");

    for (int i = 0; i <= turns; i++)
    {
        // ī���� ���ڿ� ��ȣ �Բ� ���
        switch (player[p].card_player[i].number)
        {
        case ace:
            printf("|  A   |");
            if (p == 0) {
                // ������ ���̽��� ���� ���
                if (player[0].score <= 18) {
                    player[0].score += 11;
                }
                else {
                    player[0].score += 1;
                }
            }
            break;
        case jack:
            printf("|  J   |");
            player[p].score += 10;
            break;
        case queen:
            printf("|  Q   |");
            player[p].score += 10;
            break;
        case king:
            printf("|  K   |");
            player[p].score += 10;
            break;
        default:
            printf("|  %2d  |", player[p].card_player[i].number);
            player[p].score += player[p].card_player[i].number;
            break;
        }
    }

    printf("\n");

    for (int i = 0; i <= turns; i++)
    {
        // ī���� ��ȣ ���
        switch (player[p].card_player[i].shape)
        {
        case spade:
            printf("|    %s|", SPADE);
            break;
        case club:
            printf("|    %s|", CLUB);
            break;
        case dia:
            printf("|    %s|", DIA);
            break;
        case heart:
            printf("|    %s|", HEART);
            break;
        }
    }

    printf("\n");

    for (int i = 0; i <= turns; i++)
    {
        printf("------- "); // ī�� ������ �� ���
    }
    printf("\n");

    printf("score : %d\n", player[p].score);

    player[p].bust = player[p].score > 21;

    return;
}

int betting(struct Player player)
{
    int bet;

    printf("���� �ɾ�ô�! %d �� �󸶸� �����Ͻðڽ��ϱ�? ", player.cash);
    scanf("%d", &bet);

    if (bet > player.cash)
    {
        printf("�̷��̷�... ���� �����ϱ���. ���ݸ� �Ʋ��߰ھ��. �ٽ� �Է� : ");
        scanf("%d", &bet);
        return bet;
    }
    else return bet;
}

// ������ ����
void stayorhit(int p, int turns)
{
    char answer;

    printf("\n##########TURN : PLAYER%d��##########\n\n", p);
    while (player[p].score <= 21)
    {
        printf("HIT�� ���Ͻø� h��, STAY�� ���Ͻø� s�� �Է��ϼ���[h/s]: ");
        scanf(" %c", &answer);

        if (answer == 'h' || answer == 'H')
        {
            turns++;
            player[p].card_player[turns] = deal(next++);
            printcard(p, turns);
            if (player[p].score > 21) {
                printf("21�� �ʰ��Ǿ����ϴ�. �й�\n");
                break;
            }
        }

        else if (answer == 's' || answer == 'S')
        {
            // stay�� �Է��� ���Ŀ� ace ������ ó���մϴ�.
            for (int i = 0; i <= turns; i++)
            {
                if (player[p].card_player[i].number == ace)
                {
                    printf("���̽� ī���� ������ �����ϼ��� [1/11]: ");
                    int ace_value;
                    scanf("%d", &ace_value);
                    if (ace_value == 11)
                    {
                        player[p].score += 11; // ���̽��� �̹� 1�� ó���Ǿ� �����Ƿ� 11�� �߰��մϴ�.
                    }
                    else if (ace_value == 1) {
                        player[p].score += 1;
                    }
                    break; // �̹� ó���� ���̽��� ������ ���������Ƿ� �ݺ��� ����
                }
            }
            break;
        }
        else
        {
            printf("HIT�̸� h, STAY�� s��ϱ��.\n�ٽ�,");
        }
    }
}

// ������ ���������� 17������ �����ϰ� ����� hit
void dealer(int turns)
{
    printf("\n##########DEALER's TURN##########\n\n");
    while (player[0].score < 21)
    {
        if (player[0].score < AI)
        {
            turns++;
            player[0].card_player[turns] = deal(next++);
            printcard(0, turns);
        }
        else
        {
            break;
        }
    }
    if (player[0].score > 21) player[0].score = 0;
}

// ���� ����
void play()
{
    int turns = 1; // �� ���� ī��� �����ϱ� ������ �ʱ� ���� 1�� ����
    int push_flag = 0;
    int bet_total = 0;

    while (1)
    {
        int winner = -1; // -1: no winner, 0: dealer, 1: player
        int winnerscore = -1;
        int bet[2];

        if (push_flag == 0) bet_total = 0;

        // �÷��̾� Ȯ��
        if (player[1].cash <= 0)
        {
            printf("!!! PLAYER1 �Ļ��ϼ̽��ϴ�. 100 �ٽ� ����\n\n");
            player[1].cash += 100;
        }

        player[0].score = 0;
        player[1].score = 0;

        player[0].card_player[0] = deal(next++);
        player[0].card_player[1] = deal(next++);
        printcard(0, turns);

        printf("\n<<PLAYER1>>\n");
        printf("CASH : %d\n", player[1].cash);
        player[1].card_player[0] = deal(next++);
        player[1].card_player[1] = deal(next++);
        printcard(1, turns);

        printf("\nPLAYER1��, ");
        bet[1] = betting(player[1]);
        bet[0] = bet[1]; // ������ ������ �÷��̾�� �����ϰ� ����
        bet_total = bet[0] + bet[1];

        stayorhit(1, turns);

        dealer(turns);

        printf("\n���� ���>> DEALER: %d / PLAYER1: %d\n", player[0].score, player[1].score);

        // ���� ����� ���� ó��
        if (player[0].score > 21 && player[1].score > 21) {
            printf("�� �� 21���� �ʰ��߽��ϴ�. ������ �¸��Դϴ�.\n");
            winner = 0;
        }
        else if (player[0].score > 21) {
            printf("������ 21���� �ʰ��߽��ϴ�. �÷��̾��� �¸��Դϴ�.\n");
            winner = 1;
        }
        else if (player[1].score > 21) {
            printf("�÷��̾ 21���� �ʰ��߽��ϴ�. ������ �¸��Դϴ�.\n");
            winner = 0;
        }
        else if (player[0].score == player[1].score) {
            printf("�����ϴ�.\n");
            push_flag = 1;
        }
        else if (player[0].score > player[1].score) {
            winner = 0;
        }
        else {
            winner = 1;
        }

        printf("prize: ");

        // ���� ����� ���� �ݾ� ó��
        if (push_flag) {
            // ��� ��� ���ñݾ��� ���� ����
            printf("0\n");
        }
        else if (winner == 0) {
            player[1].cash -= bet[1];
            printf("%d\n", bet_total);
        }
        else if (winner == 1) {
            player[1].cash += bet[1];
            printf("%d\n", bet_total);
        }

        printf("\n���>> P1 CASH : %d\n", player[1].cash);

        // �÷��̾��� cash�� �� �谡 �Ǹ� ������ ����
        if (player[1].cash >= player[1].initial_cash * 2)
        {
            printf("�����մϴ�! ����� cash�� �� �谡 �Ǿ� ������ �����մϴ�.\n");
            break;
        }

        char answer;
        printf("����Ͻðڽ��ϱ�? ");
        scanf(" %c", &answer);

        if (answer == 'y' || answer == 'Y')
        {
            continue;
        }
        else
        {
            break;
        }
    }

    printf("������ ����˴ϴ�... :)");
}

void reset()
{
    player[0].score = 0; player[1].score = 0;
    player[1].cash = 500;
    player[1].initial_cash = player[1].cash;

    filldeck(); shuffle(); next = 0;
}
void print_card_border(int length) {
    for (int i = 0; i < length; i++) {
        printf("�������������� ");
    }
    printf("\n");
}
void print_card_numbers() {
    for (int i = 1; i <= 7; i++) {
        printf("��%2d   �� ", i);
    }
    printf("\n");

    for (int i = 1; i <= 7; i++) {
        printf("��  %s �� ", (i % 4 == 1) ? SPADE : (i % 4 == 2) ? CLUB : (i % 4 == 3) ? DIA : HEART);
    }
    printf("\n");

    for (int i = 1; i <= 7; i++) {
        printf("��   %2d�� ", i);
    }
    printf("\n");
}
void print_card_numbers_bot() {
    for (int i = 1; i <= 4; i++) {
        printf("��%2d   �� ", i + 6);
    }
    printf("��J    �� ");
    printf("��Q    �� ");
    printf("��K    �� ");
    printf("\n");

    for (int i = 1; i <= 4; i++) {
        printf("��  %s �� ", ((i + 6) % 4 == 1) ? SPADE : ((i + 6) % 4 == 2) ? CLUB : ((i + 6) % 4 == 3) ? DIA : HEART);
    }
    printf("��  %s �� ", DIA);
    printf("��  %s �� ", HEART);
    printf("��  %s �� ", SPADE);
    printf("\n");

    for (int i = 1; i <= 4; i++) {
        printf("��   %2d�� ", i + 6);
    }
    printf("��    J�� ");
    printf("��    Q�� ");
    printf("��    K�� ");
    printf("\n");
}
void print_card_bottom(int length) {
    for (int i = 0; i < length; i++) {
        printf("�������������� ");
    }
    printf("\n");
}

// ��Ģ�� ����ϴ� �Լ�
void show_rules() {
    printf("\n���� ��Ģ:\n");
    printf("��������������������������������������������������������������������������������������������������������������������������\n");
    printf("��  1. ��ǥ�� ī���� ���� 21�� ������ ����� ���Դϴ�.       ��\n");
    printf("��  2. ���̽�(Ace)�� 1�� �Ǵ� 11������ ����� �� �ֽ��ϴ�.   ��\n");
    printf("��  3. ��(Jack), ��(Queen), ŷ(King)�� ���� 10���Դϴ�.      ��\n");
    printf("��  4. �ٸ� ��� ī��� ���� �״���� �����Դϴ�.            ��\n");
    printf("��  5. ī��� �÷��̾�� �������� �����ָ�,                  ��\n");
    printf("��     �÷��̾�� ��Ʈ(h)�ϰų� ������(s)�� �� �ֽ��ϴ�.     ��\n");
    printf("��  6. ������ 17�� �̻��� ������ ī�带 �޽��ϴ�.            ��\n");
    printf("��  7. 21���� �ʰ��ϸ� ����Ʈ(Bust)�� �й��ϰ� �˴ϴ�.       ��\n");
    printf("��������������������������������������������������������������������������������������������������������������������������\n\n");
}

// �޴� ����ϴ� �Լ�
void menu() {
    system("cls");
    // ī�� �迭 ��� (���κ�)
    print_card_border(7);

    // ī�� ���ڿ� ��� ���
    print_card_numbers();

    // ī�� �迭 ��� (�Ʒ��κ�)
    print_card_bottom(7);

    // �޴� ���
    printf("     ������������������������������������������������������������������������������������������\n");
    printf("     ��  ���� ���ӿ� ���� ���� ȯ���մϴ�!      ��\n");
    printf("     ��                                           ��\n");
    printf("     ��  1. ����                                  ��\n");
    printf("     ��  2. ��Ģ ����                             ��\n");
    printf("     ��  3. ����                                  ��\n");
    printf("     ������������������������������������������������������������������������������������������\n");

    // ī�� �迭 ��� (���κ�)
    print_card_border(7);

    // ī�� ���ڿ� ��� ���
    print_card_numbers_bot();

    // ī�� �迭 ��� (�Ʒ��κ�)
    print_card_bottom(7);
}

// ���� �Լ�
void start()
{
    int choice;
    do {
        menu();
        printf("�޴��� �����ϼ���: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            reset();
            play();
            break;
        case 2:
            show_rules();
            printf("\n����Ϸ��� �ƹ� Ű�� ��������...");
            getchar(); // Enter key input from previous input
            getchar(); // Wait for user input
            break;
        case 3:
            printf("������ �����մϴ�. ��������~\n");
            break;
        default:
            printf("�߸��� �����Դϴ�. �ٽ� �������ּ���.\n");
            break;
        }
    } while (choice != 3);
}

int main(void)
{
    start();

    return 0;
}
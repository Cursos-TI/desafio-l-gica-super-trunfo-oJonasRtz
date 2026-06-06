#include <stdio.h>

// Desafio Super Trunfo - Países
// Tema 1 - Cadastro das cartas
// Objetivo: No nível novato você deve criar as cartas representando as cidades utilizando scanf para entrada de dados e printf para exibir as informações.

//Colours
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"

//Card struct
typedef struct {
	char	state;
	char	code[5];
	char	city[50];
	// int		population; Population in Nivel Novato e Avancado
	unsigned long int population; // Nivel mestre
	float	area;
	float	pib;
	int		tourist_points;

	// Nivel Aventureiro
	float	population_density;
	float	pib_per_capita;

	// Nivel Mestre
	float	super_power;
}	t_card;

/**
 * Draws a line of a specified character and size.
 * @param c The character to draw.
 * @param size The number of times to draw the character.
 * 
 * This function is used to create visual separation between cards when printing their details.
*/
static void draw_line(char c, size_t size)
{
	for (size_t i = 0; i < size; i++)
		printf(YELLOW "%c" RESET, c);
	printf("\n");
}

static int error_message(const char *message)
{
	if (message)
		printf(RED "%s\n" RESET, message);
	return (1); // Return 1 to indicate an error occurred
}

/**
 * Prints the details of an array of cards.
 * @param cards The array of cards to print.
 * @param size The number of cards in the array.
 */
static void print_cards(t_card *cards, size_t size)
{
	draw_line('-', 50);
	const char* messages[10] = {
		BLUE " Estado:\t\t\t" RESET " %c\n",
		BLUE " Código:\t\t\t" RESET " %s\n",
		BLUE " Cidade: \t\t\t" RESET " %s\n",
		BLUE " População:\t\t\t" RESET " %lu\n",
		BLUE " Área:\t\t\t\t" RESET " %.2f km²\n",
		BLUE " PIB:\t\t\t\t" RESET " %.2f bilhões de reais\n",
		BLUE " Número de pontos turísticos: \t" RESET " %d\n",
		BLUE " Densidade Populacional: \t" RESET " %.2f hab/km²\n",
		BLUE " PIB per Capita: \t\t" RESET " %.2f reais\n",
		BLUE " Super Poder: \t\t\t" RESET " %.2f\n"
	};


	for (size_t i = 0; i < size; i++) {
		printf(GREEN "\t\tCarta %zu\n" RESET, i + 1);

		printf(messages[0], cards[i].state);
		printf(messages[1], cards[i].code);
		printf(messages[2], cards[i].city);
		printf(messages[3], cards[i].population);
		printf(messages[4], cards[i].area);
		printf(messages[5], cards[i].pib);
		printf(messages[6], cards[i].tourist_points);
		printf(messages[7], cards[i].population_density);
		printf(messages[8], cards[i].pib_per_capita);
		printf(messages[9], cards[i].super_power);

		draw_line('-', 50); // Draw a line after each card for better readability
	}
}

/**
 * Calculates the data for the "Nivel Aventureiro" fields of a card, which include population density and PIB per capita.
 * @param card A pointer to the card for which the values will be calculated.
 */
static void calculated_values(t_card *card){
	//Protection division by zero
	float density = (card->area == 0) ? 0.0f : (float)(card->population / card->area); // Calculate population density with protection against division by zero
	float pib = (card->population == 0) ? 0.0f : (float)(card->pib / card->population); // Calculate PIB per capita with protection against division by zero

	card->population_density = density;
	card->pib_per_capita = pib;
}

static void calculate_super_power(t_card *card) {
	float density = (card->population_density == 0) ? 1.0f : card->population_density; // Avoid division by zero for population density
	float inverse_density = 1.0f / density; // Inverse of population density
	float sum = 
		inverse_density + 
		card->population +
		card->area +
		card->pib +
		card->tourist_points +
		card->pib_per_capita;
	
	card->super_power = sum;
}

/**
 * Prompts the user to input the details for two cards and stores them in the provided array.
 * @param card An array of two t_card structures where the inputted card details will be stored.
 */
static int add_card(t_card *card, size_t size) {

	//Variables
	const char* messages[7] = {
		BLUE " | Digite o estado da cidade(A - H): " RESET,
		BLUE " | Digite o código da cidade(ex: A00): " RESET,
		BLUE " | Digite o nome da cidade: " RESET,
		BLUE " | Digite a população da cidade: " RESET,
		BLUE " | Digite a área da cidade (em km²): " RESET,
		BLUE " | Digite o PIB da cidade (em bilhões de reais): " RESET,
		BLUE " | Digite o número de pontos turísticos da cidade: " RESET
	};
	char* formats[7] = { // formats for scanf
		" %c", // space before %c to consume any leftover whitespace
		" %s",
		" %s",
		" %lu",
		" %f",
		" %f",
		" %d"
	};

	// Iterate over the number of cards and prompt the user for input for each card's fields
	for (size_t i = 0; i < size; i++) {
		void* inputs[7] = { // cast the addresses of the card fields for scanf
			&card[i].state,
			card[i].code,
			card[i].city,
			&card[i].population,
			&card[i].area,
			&card[i].pib,
			&card[i].tourist_points
		};

		printf(YELLOW "\n\t\tCadastro da Carta %zu\n" RESET, i + 1);
		for (size_t j = 0; j < 7; j++) { // loop through each field of the card and prompt for input
			printf("%s", messages[j]);
			if (scanf(formats[j], inputs[j]) != 1)
				return (1); // Return 1 if scanf fails to read the expected input
		}

		// Nivel aventureiro
		calculated_values(&card[i]);
		calculate_super_power(&card[i]);
	}

	draw_line('-', 50); // Draw a line after inputting the cards for better readability
	return (0);
}

static void battle(t_card *card1, t_card *card2) {
	// Determine the winner
	draw_line('=', 50);
	printf(GREEN "\tComparação de Cartas (Atributo: Super Poder)\n" RESET);

    printf(YELLOW "Carta 1 - " RESET "%s: %.2f\n", card1->city, card1->super_power);
    printf(YELLOW "Carta 2 - " RESET "%s: %.2f\n", card2->city, card2->super_power);

    //Compare the super power of both cards and print the result 
    printf(GREEN "\nResultado: " RESET);
    if (card1->super_power > card2->super_power)
        printf("Carta 1 (%s) venceu!\n", card1->city);
    else
        printf("Carta 2 (%s) venceu!\n", card2->city);

	
	draw_line('=', 50); // Draw a line after the battle results for better readability
}

static void menu_options() {
    printf(GREEN "\n\t\tMenu de Batalha\n" RESET);
    printf(YELLOW "1. Exibir informacoes.\n" RESET);
    printf(YELLOW "2. Comparacao por populacao.\n" RESET);
    printf(YELLOW "3. Comparacao por area.\n" RESET);
    printf(YELLOW "4. Comparacao por PIB.\n" RESET);
    printf(YELLOW "5. Comparacao por pontos turisticos.\n" RESET);
    printf(YELLOW "6. Comparacao por densidade populacional.\n" RESET);
    printf(YELLOW "7. Exibir menu de opcoes.\n" RESET);
    printf(YELLOW "8. Sair.\n" RESET);
}

static void menu(t_card *card1, t_card *card2) {
    int choice = -1;


    menu_options(); // Display the menu options at the start of the battle
    while (1) {
        printf(BLUE "$> " RESET);
        if (scanf("%d", &choice) != 1) {
            printf(RED "\nEntrada invalida. Por favor, tente novamente.\n" RESET);
            return;
        }

        switch (choice) {
            default:
                printf(RED "\nOpcao invalida. Por favor, escolha uma opcao entre 1 e 8.\n" RESET);
                break;

            case 1:
                print_cards((t_card[]){*card1, *card2}, 2);
                break;
            case 2:
                printf(GREEN "\nComparacao por Populacao\n" RESET);
                printf(YELLOW "Carta 1 - " RESET "%s: %lu habitantes\n", card1->city, card1->population);
                printf(YELLOW "Carta 2 - " RESET "%s: %lu habitantes\n", card2->city, card2->population);
                if (card1->population > card2->population)
                    printf(GREEN "\nResultado: " RESET "Carta 1 (%s) venceu!\n", card1->city);
                else if (card1->population < card2->population)
                    printf(GREEN "\nResultado: " RESET "Carta 2 (%s) venceu!\n", card2->city);
                else
                    printf(GREEN "\nResultado: " RESET "Empate!\n");
                break;
            case 3:
                printf(GREEN "\nComparacao por Area\n" RESET);
                printf(YELLOW "Carta 1 - " RESET "%s: %.2f km²\n", card1->city, card1->area);
                printf(YELLOW "Carta 2 - " RESET "%s: %.2f km²\n", card2->city, card2->area);
                if (card1->area > card2->area)
                    printf(GREEN "\nResultado: " RESET "Carta 1 (%s) venceu!\n", card1->city);
                else if (card1->area < card2->area)
                    printf(GREEN "\nResultado: " RESET "Carta 2 (%s) venceu!\n", card2->city);
                else
                    printf(GREEN "\nResultado: " RESET "Empate!\n");
                break;
            case 4:
                printf(GREEN "\nComparacao por PIB\n" RESET);
                printf(YELLOW "Carta 1 - " RESET "%s: %.2f bilhões de reais\n", card1->city, card1->pib);
                printf(YELLOW "Carta 2 - " RESET "%s: %.2f bilhões de reais\n", card2->city, card2->pib);
                if (card1->pib > card2->pib)
                    printf(GREEN "\nResultado: " RESET "Carta 1 (%s) venceu!\n", card1->city);
                else if (card1->pib < card2->pib)
                    printf(GREEN "\nResultado: " RESET "Carta 2 (%s) venceu!\n", card2->city);
                else
                    printf(GREEN "\nResultado: " RESET "Empate!\n");
                break;
            case 5:
                printf(GREEN "\nComparacao por Pontos Turisticos\n" RESET);
                printf(YELLOW "Carta 1 - " RESET "%s: %d pontos turisticos\n", card1->city, card1->tourist_points);
                printf(YELLOW "Carta 2 - " RESET "%s: %d pontos turisticos\n", card2->city, card2->tourist_points);
                if (card1->tourist_points > card2->tourist_points)
                    printf(GREEN "\nResultado: " RESET "Carta 1 (%s) venceu!\n", card1->city);
                else if (card1->tourist_points < card2->tourist_points)
                    printf(GREEN "\nResultado: " RESET "Carta 2 (%s) venceu!\n", card2->city);
                else
                    printf(GREEN "\nResultado: " RESET "Empate!\n");
                break;
            case 6:
                printf(GREEN "\nComparacao por Densidade Populacional\n" RESET);
                printf(YELLOW "Carta 1 - " RESET "%s: %.2f hab/km²\n", card1->city, card1->population_density);
                printf(YELLOW "Carta 2 - " RESET "%s: %.2f hab/km²\n", card2->city, card2->population_density);
                if (card1->population_density < card2->population_density)
                    printf(GREEN "\nResultado: " RESET "Carta 1 (%s) venceu!\n", card1->city);
                else if (card1->population_density > card2->population_density)
                    printf(GREEN "\nResultado: " RESET "Carta 2 (%s) venceu!\n", card2->city);
                else
                    printf(GREEN "\nResultado: " RESET "Empate!\n");
                break;

            case 7:
                menu_options();
                break;
            case 8:
                printf(GREEN "\nSaindo do jogo. Obrigado por jogar!\n" RESET);
                return; // Exit the menu loop and end the program
        }
    }
}

int main(void)
{
	// Variables
	const size_t size = 2;
	t_card cards[size];

	// Logic to add and print cards
	if (add_card(cards, size))
		return error_message("Erro ao ler os dados. Por favor, tente novamente.");

	// Print the details of the cards
	printf(GREEN "\n\t\tCartas Cadastradas\n" RESET);
	print_cards(cards, size);

	//Battle logic
	menu(&cards[0], &cards[1]);

	return (0);
}

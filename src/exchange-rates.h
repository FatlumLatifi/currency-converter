#ifndef EXCHANGE_RATES_H
#define EXCHANGE_RATES_H


double convert_currency(char currencyFrom[3], char currencyTo[3], double value);
void append_currencies_in_combos(GtkComboBoxText* combo1, GtkComboBoxText* combo2);
void update_currency_rates(void);
void cleanUp(void);


#endif
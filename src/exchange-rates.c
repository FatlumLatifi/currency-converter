#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include <gtk/gtk.h>

#ifndef ASSETS_DIR
#define ASSETS_DIR "../assets/"
#endif

#define ECB_FILE ASSETS_DIR "ecb.xml"

xmlDoc* _docPtr = NULL;

void cleanUp(void) { xmlFreeDoc(_docPtr); }

typedef enum ConvertType { FromEUR = 0, ToEUR = 1 } ConvertType;

// Callback function to handle the response data
size_t write_callback(void* ptr, size_t size, size_t nmemb, void* userdata) 
{
  int total = size*nmemb;
  xmlFreeDoc(_docPtr);
  _docPtr = NULL;

  FILE* file = fopen(ECB_FILE, "w");
  int outputLength = fprintf(file, "%.*s", total, (char*)ptr);
    if (outputLength < total) { printf("not fully written on file"); }
    fclose(file);
  _docPtr = xmlReadFile(ECB_FILE, NULL, 0);
  return total;
}

void update_currency_rates(void)
{
    CURL* curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.ecb.europa.eu/stats/eurofxref/eurofxref-daily.xml");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK) { fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res)); }
       
        curl_easy_cleanup(curl);
    }
}



void append_currencies_from_node(GtkComboBoxText* combo1, GtkComboBoxText* combo2, xmlNode* current_node)
{
   
}

xmlNode* move_next_node(xmlNode* current_node)
{
     if (current_node->children) { current_node = current_node->children; } 
        else 
        {  while (current_node && !current_node->next) 
        { current_node = current_node->parent;  }
            if (current_node) { current_node = current_node->next; }
        }
        return current_node;
}

void append_currencies_in_combos(GtkComboBoxText* combo1, GtkComboBoxText* combo2)
{
    xmlChar* attrValue = NULL;
    xmlNode* current_node = xmlDocGetRootElement(_docPtr);
    while (current_node) 
    {
        attrValue = NULL;
        if (current_node->type == XML_ELEMENT_NODE && strcmp(current_node->name, "Cube") == 0) 
            {
                attrValue = xmlGetProp(current_node, (const xmlChar*)"currency");
                if (attrValue)
                {
                    g_print("Appending: %s\n", attrValue);
                    gtk_combo_box_text_append_text(combo1, (char*)attrValue);
                    gtk_combo_box_text_append_text(combo2, (char*)attrValue);
                }
            }
        current_node = move_next_node(current_node);
    }
    xmlFree(current_node);
    xmlFree(attrValue);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo1), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo2), 1);
}

double convert_euro(ConvertType type, char currencyName[3], double value)
{
    xmlNode* node = xmlDocGetRootElement(_docPtr);
    xmlChar* attrValue = NULL;
    double result;

    while (node) 
    {
        //attrValue = NULL;
        if (node->type == XML_ELEMENT_NODE && strcmp(node->name, "Cube") == 0) 
        {
            attrValue = xmlGetProp(node, (const xmlChar*)"currency");
            if (attrValue)
            {
                if (strcmp(attrValue, currencyName) == 0)
                {
                    xmlChar* rateValue = xmlGetProp(node, (const xmlChar*)"rate");
                    if (rateValue)
                    {
                        double rateDouble = strtod(rateValue, NULL);
                        xmlFree(rateValue);
                        if (type == FromEUR) { result = value*rateDouble; }
                        else { result = value/rateDouble; }
                        break;
                    }
                }
            }
        }
        node = move_next_node(node);
    }
//xmlFree(node);
xmlFree(attrValue);
return result;
}



double convert_currency(char currencyFrom[3], char currencyTo[3], double value)
{
    if (_docPtr == NULL)
    {
        _docPtr = xmlReadFile(ECB_FILE, NULL, 0);
       // root_node = xmlDocGetRootElement(doc);
    }
     
    if (currencyFrom == currencyTo) { return value; }

    if (strcmp(currencyFrom, "EUR") == 0)
    {
        return convert_euro(FromEUR, currencyTo, value);
    }
    else if (strcmp(currencyTo, "EUR") == 0)
    {
        return convert_euro(ToEUR, currencyFrom, value);
    }
    else
    {
        double euroValue = convert_euro(ToEUR, currencyFrom, value);
        return convert_euro(FromEUR, currencyTo, euroValue);
    }
}



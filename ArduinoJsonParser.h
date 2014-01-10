/*
 * malloc-free JSON parser for Arduino
 * Benoit Blanchon 2014
 * MIT License
 */

#ifndef __ARDUINOJSONPARSER_H
#define __ARDUINOJSONPARSER_H

#include <Arduino.h>
#include "utility/jsmn.h"

class JsonParserBase
{
public:

	JsonParserBase()
	{
		jsmn_init(&parser);
	}

protected:

	bool parseTokens(char* jsonString, jsmntok_t* tokens, int tokenCount)
	{
		buffer = jsonString;

		if (JSMN_SUCCESS != jsmn_parse(&parser, jsonString, tokens, tokenCount))
			return false;

		// Add null termination to each token
		for (int i = 0; i < tokenCount; i++)
		{
			buffer[tokens[i].end] = 0;
		}

		return true;
	}

	char* getValueByKey(char* name, jsmntok_t* tokens, int tokenCount)
	{
		// Scan each keys, every two other token
		// (skip index 0, because it's the whole json object)
		for (int i = 1; i < tokenCount; i += 2)
		{
			// Early break if we reach the last token
			if (i >= parser.toknext) break;

			// Get key token string
			char* key = buffer + tokens[i].start;

			// Compare with desired name
			if (strcmp(name, key) == 0)
			{
				return buffer + tokens[i + 1].start;
			}
		}
	}

private:

	char* buffer;
	jsmn_parser parser;
};

template <int N>
class ArduinoJsonParser : JsonParserBase
{
public:

	bool parse(char* json)
	{
		return parseTokens(json, tokens, N * 2 + 1);
	}
	
	char* getValue(char* name)
	{
		return getValueByKey(name, tokens, N * 2 + 1);
	}

private:
	
	jsmntok_t tokens[N * 2 + 1];
};

#endif


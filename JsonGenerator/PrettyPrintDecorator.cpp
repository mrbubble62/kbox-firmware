/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "PrettyPrintDecorator.h"

size_t PrettyPrintDecorator::write(uint8_t c)
{
    size_t n = inString ? handleStringChar(c) : handleMarkupChar(c);    
    previousChar = c;
    return n;
}

size_t PrettyPrintDecorator::handleStringChar(uint8_t c)
{
    bool isQuote = c == '"' && previousChar != '\\';

    if (isQuote) inString = false;

    return writeChar(c);
}

size_t PrettyPrintDecorator::handleMarkupChar(uint8_t c)
{
    switch (c)
    {
    case '{':
    case '[':
        return handleBlockOpen(c);

    case '}':
    case ']':
        return handleBlockClose(c);

    case ':':
        return handleColumn();

    case ',':
        return handleComma();

    case '"':
        return handleQuoteOpen();

    default:
        return handleNormalChar(c);
    }
}

size_t PrettyPrintDecorator::handleBlockOpen(uint8_t c)
{
    size_t n = inEmptyBlock() ? breakThenWrite(c) : writeChar(c);

    indent++;

    return n;
}

size_t PrettyPrintDecorator::handleBlockClose(uint8_t c)
{
    indent--;

    return inEmptyBlock() ? writeChar(c) : breakThenWrite(c);
}

size_t PrettyPrintDecorator::handleColumn()
{
    return writeChar(':') + writeChar(' ');
}

size_t PrettyPrintDecorator::handleComma()
{
    return writeThenBreak(',');
}

size_t PrettyPrintDecorator::handleQuoteOpen()
{
    size_t n = inEmptyBlock() ? breakThenWrite('"') : writeChar('"');

    inString = true;

    return n;
}

size_t PrettyPrintDecorator::handleNormalChar(uint8_t c)
{
    return inEmptyBlock() ? breakThenWrite(c) : writeChar(c);
}

size_t PrettyPrintDecorator::breakAndIndent()
{
    size_t n = writeChar('\n');

    for (int i = 0; i < indent; i++)
        n += writeChar(' ');

    return n;
}
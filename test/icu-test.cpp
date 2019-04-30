#include <unicode/unistr.h>
#include <unicode/brkiter.h>
#include <unicode/uloc.h>
#include <unicode/utypes.h>

#include <memory>
#include <iostream>

int main() {
    std::string s;

    std::getline(std::cin, s);
    auto us = icu::UnicodeString::fromUTF8(s);
    UErrorCode errc;
    auto brk = std::unique_ptr<icu::BreakIterator>{icu::BreakIterator::createCharacterInstance(icu::Locale::getUS(), errc)};

    /*
    int32_t n;
    const icu::Locale* loc = icu::Locale::getAvailableLocales(n);
    for (int i = 0; i < n; ++i) {
        std::cout << loc[i].getName() << std::endl;
    }
    */

    std::cout << icu::Locale::getDefault().getName() << std::endl;
    brk->setText(us);
    for (int32_t i = brk->first(); i != icu::BreakIterator::DONE; i = brk->next()) {
        std::cout << "boundary at position " << i << std::endl;
    }
}

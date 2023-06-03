/*
 * Copyright (C) 2023- Christian Stieber
 */

#include <locale>
#include <cstring>

/************************************************************************/

namespace HTMLParser
{
    class UTF8Codecvt : public std::codecvt<char32_t, char8_t, std::mbstate_t>
    {
    private:
        state_type state;

    public:
        UTF8Codecvt():
            codecvt(1)
        {
            memset(&state, 0, sizeof(state));
        }

        virtual ~UTF8Codecvt() =default;

    public:
        std::u32string doConvert(std::string_view input)
        {
            std::u32string output;
            output.resize(input.size());

            const char8_t* from=static_cast<const char8_t*>(static_cast<const void*>(input.data()));
            const char8_t* from_end=static_cast<const char8_t*>(static_cast<const void*>(input.data()+input.size()));
            const char8_t* from_next;
            char32_t* to_next;

            auto result=in(state, from, from_end, from_next, output.data(), output.data()+output.size(), to_next);

            if (result==std::codecvt_base::result::ok &&
                from_next==from_end &&
                mbsinit(&state))
            {
                output.resize(to_next-output.data());
                return output;
            }
            else
            {
                assert(result!=std::codecvt_base::result::noconv);
                throw ConversionException();
            }
        }

        std::string doConvert(std::u32string_view input)
        {
            std::string output;
            output.resize(4*input.size());

            const char32_t* from_end=input.data()+input.size();
            const char32_t* from_next;
            char8_t* to=static_cast<char8_t*>(static_cast<void*>(output.data()));
            char8_t* to_end=static_cast<char8_t*>(static_cast<void*>(output.data()+output.size()));
            char8_t* to_next;

            auto result=out(state, input.data(), from_end, from_next, to, to_end, to_next);

            if (result==std::codecvt_base::result::ok &&
                from_next==from_end &&
                mbsinit(&state))
            {
                output.resize(to_next-to);
                return output;
            }
            else
            {
                assert(result!=std::codecvt_base::result::noconv);
                throw ConversionException();
            }
        }

    public:
        static std::u32string convert(std::string_view input)
        {
            return UTF8Codecvt().doConvert(input);
        }

        static std::string convert(std::u32string_view input)
        {
            return UTF8Codecvt().doConvert(input);
        }
    };
}

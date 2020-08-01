#pragma once
#ifndef __HTML_ENTITIES_DECODER__
#define __HTML_ENTITIES_DECODER__

#include <climits>
#include <clocale>
#include <cuchar>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace html_entities_decoder
{
	class html_entities_decoder
	{
	private:
#if defined(_WIN32) || defined(WIN32)
		const bool is_windows = true;
#else
		const bool is_windows = false;
#endif

	protected:

		std::wstring string_to_wstring(const std::string &input)
		{
			std::wstring converted_string;
			mbstate_t state{};
			const char *mbstr = input.c_str();

			char *current_locale = setlocale(LC_ALL, nullptr);
			bool default_c_local = strcmp(current_locale, "C") == 0;
			if (default_c_local)
				setlocale(LC_ALL, "");

			size_t length = mbsrtowcs(NULL, &mbstr, 0, &state) + 1;
			std::vector<wchar_t> vector_wchar(length, 0);
			mbsrtowcs(&vector_wchar[0], &mbstr, vector_wchar.size(), &state);

			if (default_c_local)
				setlocale(LC_ALL, "C");

			converted_string = vector_wchar.data();
			return converted_string;
		}

		std::string wstring_to_string(const std::wstring &input)
		{
			std::string converted_string;
			std::vector<char> vector_char(input.size() * 3, 0);

			char *current_locale = setlocale(LC_ALL, nullptr);
			bool default_c_local = strcmp(current_locale, "C") == 0;
			if (default_c_local)
				setlocale(LC_ALL, "");

			wcstombs(&vector_char[0], input.c_str(), vector_char.size());
			
			if (default_c_local)
				setlocale(LC_ALL, "C");
			
			converted_string = vector_char.data();
			return converted_string;
		}

#if __cplusplus >= 202002L

		std::u8string string_to_u8string(const std::string &input)
		{
			std::u8string converted_string;
			const char *start_ptr = input.c_str();
			const char *end_ptr = start_ptr + input.size();
			int return_value;
			mbstate_t state{};
			std::vector<char8_t> vector_ch8(input.size() + 1, 0);

			char *current_locale = setlocale(LC_ALL, nullptr);
			bool default_c_local = strcmp(current_locale, "C") == 0;
			if (default_c_local)
				setlocale(LC_ALL, "");

			for (char8_t *ch8_ptr = &vector_ch8[0]; (return_value = mbrtoc8(ch8_ptr, start_ptr, end_ptr - start_ptr, &state)); ++ch8_ptr, start_ptr += return_value)
			{
				if (return_value > static_cast<size_t>(-1) / 2) break;
			}

			if (default_c_local)
				setlocale(LC_ALL, "C");

			converted_string = vector_ch8.data();
			return converted_string;
		}

		std::string u8string_to_string(const std::u8string &input)
		{
			std::string converted_string;
			mbstate_t state{};

			char *current_locale = setlocale(LC_ALL, nullptr);
			bool default_c_local = strcmp(current_locale, "C") == 0;
			if (default_c_local)
				setlocale(LC_ALL, "");

			for (char8_t u8ch : input)
			{
				char ansi_char[MB_LEN_MAX]{};
				size_t rc = c8rtomb(ansi_char, u8ch, &state);
				if (rc != static_cast<size_t>(-1))
				{
					converted_string += std::string{ ansi_char , rc };
				}
			}

			if (default_c_local)
				setlocale(LC_ALL, "C");

			return converted_string;
		}

		std::u8string wstring_to_u8string(const std::wstring &input)
		{
			std::u8string converted_string;
			mbstate_t state{};
			for (char16_t ch16 : input)
			{
				char u8char[MB_LEN_MAX]{};
				size_t rc = c16rtomb(u8char, ch16, &state);
				if (rc != static_cast<size_t>(-1))
				{
					converted_string += std::u8string{ reinterpret_cast<char8_t *>(u8char) , rc };
				}
			}
			return converted_string;
		}

		std::wstring u8string_to_wstring(const std::u8string &input)
		{
			std::wstring converted_string;
			mbstate_t state{};
			const char *start_ptr = reinterpret_cast<const char *>(input.c_str());
			const char *end_ptr = start_ptr + input.size();
			std::vector<char16_t> vector_ch16(input.size() + 1, 0);
			long long count;
			for (char16_t *ch16_ptr = &vector_ch16[0]; (ret = mbrtoc16(ch16_ptr, start_ptr, end_ptr - start_ptr, &state)); ++ch16_ptr, start_ptr += count)
			{
				if (count > static_cast<size_t>(-1) / 2) break;
			}
			converted_string = reinterpret_cast<wchar_t *>(vector_ch16.data());
			return converted_string;
		}

		std::u8string u16string_to_u8string(const std::u16string &input)
		{
			std::u8string converted_string;
			mbstate_t state{};
			for (char16_t ch16 : input)
			{
				char u8char[MB_LEN_MAX]{};
				size_t rc = c16rtomb(u8char, ch16, &state);
				if (rc != static_cast<size_t>(-1))
				{
					converted_string += std::u8string{ reinterpret_cast<char8_t *>(u8char) , rc };
				}
			}
			return converted_string;
		}

		std::u16string u8string_to_u16string(const std::u8string &input)
		{
			std::u16string converted_string;
			mbstate_t state{};
			const char *start_ptr = reinterpret_cast<const char *>(input.c_str());
			const char *end_ptr = start_ptr + input.size();
			std::vector<char16_t> vector_ch16(input.size() + 1, 0);
			long long count;
			for (char16_t *ch16_ptr = &vector_ch16[0]; (ret = mbrtoc16(ch16_ptr, start_ptr, end_ptr - start_ptr, &state)); ++ch16_ptr, start_ptr += count)
			{
				if (count > static_cast<size_t>(-1) / 2) break;
			}
			converted_string = vector_ch16.data();
			return converted_string;
		}

#endif

		std::u16string string_to_u16string(const std::string &input)
		{
			std::u16string converted_string;
			mbstate_t state{};

			char *current_locale = setlocale(LC_ALL, nullptr);
			bool default_c_local = strcmp(current_locale, "C") == 0;
			if (default_c_local)
				setlocale(LC_ALL, "");
			
			if (is_windows || !default_c_local)
			{
				const char *mbstr = input.c_str();
				size_t length = mbsrtowcs(NULL, &mbstr, 0, &state) + 1;
				std::vector<wchar_t> vector_wchar(length, 0);
				mbsrtowcs(&vector_wchar[0], &mbstr, vector_wchar.size(), &state);
				converted_string = reinterpret_cast<char16_t *>(vector_wchar.data());
			}
			else
			{
				const char *start_ptr = input.c_str();
				const char *end_ptr = start_ptr + input.size();
				std::vector<char16_t> vector_ch16(input.size() + 1, 0);
				long long count;
				for (char16_t *ch16_ptr = &vector_ch16[0]; (count = mbrtoc16(ch16_ptr, start_ptr, end_ptr - start_ptr, &state)); ++ch16_ptr, start_ptr += count)
					if (count > static_cast<size_t>(-1) / 2) break;
				converted_string = vector_ch16.data();
			}

			if (default_c_local)
				setlocale(LC_ALL, "C");

			return converted_string;
		}

		std::string u16string_to_string(const std::u16string &input)
		{
			std::string converted_string;
			mbstate_t state{};

			if (is_windows)
			{
				std::vector<char> vector_char(input.size() * 3, 0);

				char *current_locale = setlocale(LC_ALL, nullptr);
				bool default_c_local = strcmp(current_locale, "C") == 0;
				if (default_c_local)
					setlocale(LC_ALL, "");

				wcstombs(&vector_char[0], reinterpret_cast<const wchar_t *>(input.c_str()), vector_char.size());

				if (default_c_local)
					setlocale(LC_ALL, "C");

				converted_string = vector_char.data();
			}
			else
			{
				for (char16_t ch16 : input)
				{
					char u8char[MB_LEN_MAX]{};
					size_t rc = c16rtomb(u8char, ch16, &state);
					if (rc != static_cast<size_t>(-1))
					{
						converted_string += std::string{ u8char , rc };
					}
				}
			}

			return converted_string;
		}

		std::u16string wstring_to_u16string(const std::wstring &input)
		{
			std::u16string converted_string(input.begin(), input.end());
			return converted_string;
		}

		std::wstring u16string_to_wstring(const std::u16string &input)
		{
			std::wstring converted_string(input.begin(), input.end());
			return converted_string;
		}

		std::u32string string_to_u32string(const std::string &input)
		{
			std::u32string converted_string;
			std::vector<char32_t> vector_ch32(input.size() + 1, 0);
			mbstate_t state{};
			const char *start_ptr = input.data();
			const char *end_ptr = start_ptr + input.size();
#if __cplusplus >= 202002L
			std::vector<char8_t> vector_ch8(input.size() + 1, 0);
#else
			std::string ch8s_input;
#endif			

			char *current_locale = setlocale(LC_ALL, nullptr);
			bool default_c_local = strcmp(current_locale, "C") == 0;
			if (default_c_local)
				setlocale(LC_ALL, "");

			if (is_windows || !default_c_local)
			{
#if __cplusplus >= 202002L
				int return_value;
				for (char8_t *ch8_ptr = &vector_ch8[0]; (return_value = mbrtoc8(ch8_ptr, start_ptr, end_ptr - start_ptr, &state)); ++ch8_ptr, start_ptr += return_value)
				{
					if (return_value > static_cast<size_t>(-1) / 2) break;
				}

				start_ptr = reinterpret_cast<char *>(vector_ch8.data());
				end_ptr = start_ptr + vector_ch8.size();
#else
				state = mbstate_t();
				const char *mbstr = input.c_str();
				size_t length = mbsrtowcs(NULL, &mbstr, 0, &state) + 1;
				std::vector<wchar_t> vector_wchar(length, 0);
				mbsrtowcs(&vector_wchar[0], &mbstr, vector_wchar.size(), &state);

				state = mbstate_t();
				for (wchar_t wch : vector_wchar)	// convert to UTF8 string
				{
					char16_t ch16 = wch;
					char u8char[MB_LEN_MAX]{};
					size_t rc = c16rtomb(u8char, ch16, &state);
					if (rc != static_cast<size_t>(-1))
					{
						ch8s_input += std::string{ u8char , rc };
					}
				}

				start_ptr = ch8s_input.data();
				end_ptr = start_ptr + ch8s_input.size();
#endif
				if (default_c_local)
					setlocale(LC_ALL, "C");
			}

			long long count;
			state = mbstate_t();
			for (char32_t *ch32_ptr = &vector_ch32[0]; (count = mbrtoc32(ch32_ptr, start_ptr, end_ptr - start_ptr, &state)); ++ch32_ptr, start_ptr += count)
			{
				if (count > static_cast<size_t>(-1) / 2) break;
			}
			converted_string = vector_ch32.data();
			return converted_string;
		}

		std::string u32string_to_string(const std::u32string &input)
		{
			std::string converted_string;
			mbstate_t state{};

			for (char32_t ch32 : input)
			{
				char ch[MB_LEN_MAX]{};
				size_t rc = std::c32rtomb(ch, ch32, &state);
				if (rc != static_cast<size_t>(-1))
				{
					converted_string += std::string{ ch, rc };
				}
			}
			state = mbstate_t();

			char *current_locale = setlocale(LC_ALL, nullptr);
			bool default_c_local = strcmp(current_locale, "C") == 0;
			if (default_c_local)
				setlocale(LC_ALL, "");

			if (is_windows || !default_c_local)
			{
#if __cplusplus >= 202002L

				std::string ansi_string;
				for (char u8ch : converted_string)	// convert to ANSI string
				{
					char8_t ch8 = u8ch;
					char ansi_char[MB_LEN_MAX]{};
					size_t rc = c8rtomb(ansi_char, ch8, &state);
					if (rc != static_cast<size_t>(-1))
					{
						ansi_string += std::string{ ansi_char , rc };
					}
				}
				converted_string = ansi_string;
#else
				const char *start_ptr = converted_string.data();
				const char *end_ptr = start_ptr + converted_string.size();
				std::vector<char16_t> vector_ch16(converted_string.size() + 1, 0);
				long long count;
				for (char16_t *ch16_ptr = &vector_ch16[0]; (count = mbrtoc16(ch16_ptr, start_ptr, end_ptr - start_ptr, &state)); ++ch16_ptr, start_ptr += count)
				{
					if (count > static_cast<size_t>(-1) / 2) break;
				}

				std::vector<char> vector_char(converted_string.size() + 1, 0);
				wcstombs(&vector_char[0], reinterpret_cast<wchar_t *>(vector_ch16.data()), vector_char.size());
				converted_string = vector_char.data();
#endif
				if (strcmp(current_locale, "C") == 0)
					setlocale(LC_ALL, "C");
			}
			return converted_string;
		}

#if __cplusplus >= 202002L

		std::u32string u8string_to_u32string(const std::u8string &input)
		{
			std::u32string converted_string;
			std::vector<char32_t> vector_ch32(input.size() + 1, 0);			
			const char *start_ptr = reinterpret_cast<const char *>(input.data());
			const char *end_ptr = start_ptr + input.size();
			mbstate_t state{};
			int count;
			for (char32_t *ch32_ptr = &vector_ch32[0]; (ret = mbrtoc32(ch32_ptr, start_ptr, end_ptr - start_ptr, &state)); ++ch32_ptr, start_ptr += count)
			{
				if (count > static_cast<size_t>(-1) / 2) break;

			}
			converted_string = vector_ch32.data();
			return converted_string;
		}

		std::u8string u32string_to_u8string(const std::u32string &input)
		{
			std::u8string converted_string;
			mbstate_t state{};

			for (char32_t ch32 : input)
			{
				char8_t ch[MB_LEN_MAX]{};
				size_t rc = std::c32rtomb(reinterpret_cast<char *>(ch), ch32, &state);
				if (rc != static_cast<size_t>(-1))
				{
					converted_string += std::u8string{ ch, rc };
				}
			}

			return converted_string;
		}

#endif

		std::u32string wstring_to_u32string(const std::wstring &input)
		{
			std::u32string converted_string;
			mbstate_t state{};
			for (wchar_t wch : input)	// convert to UTF8 string
			{
				char16_t ch16 = wch;
				char u8char[MB_LEN_MAX]{};
				size_t rc = c16rtomb(u8char, ch16, &state);
				if (rc != static_cast<size_t>(-1))
				{
					mbstate_t state{};
					char32_t ch32;
					long long count = mbrtoc32(&ch32, u8char, rc, &state);
					if (count > static_cast<size_t>(-1) / 2) break;
					else converted_string += ch32;
				}
			}

			return converted_string;
		}

		std::wstring u32string_to_wstring(const std::u32string &input)
		{
			std::wstring converted_string;
			mbstate_t state{};
			for (char32_t ch32 : input)
			{
				char ch[MB_LEN_MAX]{};
				size_t rc = std::c32rtomb(ch, ch32, &state);
				if (rc != static_cast<size_t>(-1))
				{
					mbstate_t state{};
					char16_t ch16;
					long long count = mbrtoc16(&ch16, ch, rc, &state);
					if (count > static_cast<size_t>(-1) / 2) break;
					else converted_string += ch16;
				}
			}

			return converted_string;
		}

		std::u32string u16string_to_u32string(const std::u16string &input)
		{
			std::u32string converted_string;
			mbstate_t state{};
			for (char16_t ch16 : input)	// convert to UTF8 string
			{
				char u8char[MB_LEN_MAX]{};
				size_t rc = c16rtomb(u8char, ch16, &state);
				if (rc != static_cast<size_t>(-1))
				{
					mbstate_t state{};
					char32_t ch32;
					long long count = mbrtoc32(&ch32, u8char, rc, &state);
					converted_string += ch32;
				}
			}

			return converted_string;
		}

		std::u16string u32string_to_u16string(const std::u32string &input)
		{
			std::u16string converted_string;
			mbstate_t state{};
			for (char32_t ch32 : input)
			{
				char ch[MB_LEN_MAX]{};
				size_t rc = std::c32rtomb(ch, ch32, &state);
				if (rc != static_cast<size_t>(-1))
				{
					mbstate_t state{};
					char16_t ch16;
					long long count = mbrtoc16(&ch16, ch, rc, &state);
					converted_string += ch16;
				}
			}

			return converted_string;
		}


		template <typename ForwardIteratorT>
		auto decode_begin(ForwardIteratorT InputBegin, ForwardIteratorT InputEnd)
		{
			auto source_char = *InputBegin;
			std::u32string text_string;
			std::basic_string<decltype(source_char)> input_string(InputBegin, InputEnd);
			std::basic_string<decltype(source_char)> output_string;

			if constexpr (std::is_same_v<decltype(source_char), char>)
			{
				text_string = string_to_u32string(input_string);
			}
#if __cplusplus >= 202002L
			if constexpr (std::is_same_v<decltype(source_char), char8_t>)
			{				
				text_string = u8string_to_u32string(input_string);
			}
#endif
			if constexpr (std::is_same_v<decltype(source_char), wchar_t>)
			{
				text_string = wstring_to_u32string(input_string);
			}
			if constexpr (std::is_same_v<decltype(source_char), char16_t>)
			{
				text_string = u16string_to_u32string(input_string);
			}
			if constexpr (std::is_same_v<decltype(source_char), char32_t>)
			{
				text_string = input_string;
			}

			for (size_t and_symbol_position = text_string.find(U'&');
				and_symbol_position != text_string.npos;
				and_symbol_position = text_string.find(U'&', and_symbol_position + 1))
			{
				if (size_t semicolon_position = text_string.find(U';', and_symbol_position);
					semicolon_position != text_string.npos)
				{
					std::u32string encoded_string = text_string.substr(and_symbol_position + 1, semicolon_position - and_symbol_position - 1);	// substr exclude '&', ';'
					if (encoded_string.size() == 0)
						continue;
					if (U'#' == encoded_string[0])
					{
						char32_t replace_ch[2]{};
						if (U'x' == encoded_string[1] || U'X' == encoded_string[1])
						{
							std::string hex_number_string = u32string_to_string(encoded_string);
							hex_number_string[0] = '0';
							unsigned long ch = std::stoul(hex_number_string, 0, 16);
							replace_ch[0] = static_cast<char32_t>(ch);
						}
						else
						{
							std::string dec_number_string = u32string_to_string(encoded_string.substr(1));
							unsigned long ch = std::stoul(dec_number_string, 0, 10);
							replace_ch[0] = static_cast<char32_t>(ch);
						}

						text_string.replace(and_symbol_position, semicolon_position - and_symbol_position + 1, replace_ch);
					}
					else if (auto entity_pair = entities_map.find(encoded_string); entity_pair != entities_map.end())
					{
						text_string.replace(and_symbol_position, semicolon_position - and_symbol_position + 1, entity_pair->second);
					}
				}
				else
				{
					break;
				}
			}

			if constexpr (std::is_same_v<decltype(source_char), char>)
			{
				output_string = u32string_to_string(text_string);
			}
#if __cplusplus >= 202002L
			if constexpr (std::is_same_v<decltype(source_char), char8_t>)
			{
				output_string = u32string_to_u8string(text_string);
			}
#endif
			if constexpr (std::is_same_v<decltype(source_char), wchar_t>)
			{
				output_string = u32string_to_wstring(text_string);
			}
			if constexpr (std::is_same_v<decltype(source_char), char16_t>)
			{
				output_string = u32string_to_u16string(text_string);
			}
			if constexpr (std::is_same_v<decltype(source_char), char32_t>)
			{
				output_string = text_string;
			}

			return output_string;
		}

	public:

		template<typename _CharType>
		auto decode_html_entities(const _CharType &input)
		{
			auto result_string = decode_begin(cbegin(input), cend(input));

			return result_string;
		}

		template<typename _CharType>
		std::basic_string<_CharType> decode_html_entities(const _CharType *input, size_t N)
		{
			std::basic_string<_CharType> str(input, N);

			std::basic_string<_CharType> result_string = decode_begin(cbegin(str), cend(str));

			return result_string;
		}

	private:

		const std::map<std::u32string, std::u32string> entities_map
		{
			{ U"Tab", U"\t" },
			{ U"NewLine", U"\n" },
			{ U"excl", U"\u0021" },
			{ U"quot", U"\u0022" },
			{ U"QUOT", U"\u0022" },
			{ U"num", U"\u0023" },
			{ U"dollar", U"\u0024" },
			{ U"percnt", U"\u0025" },
			{ U"amp", U"\u0026" },
			{ U"AMP", U"\u0026" },
			{ U"apos", U"\u0027" },
			{ U"lpar", U"\u0028" },
			{ U"rpar", U"\u0029" },
			{ U"ast", U"\u002A" },
			{ U"midast", U"\u002A" },
			{ U"plus", U"\u002B" },
			{ U"comma", U"\u002C" },
			{ U"period", U"\u002E" },
			{ U"sol", U"\u002F" },
			{ U"colon", U"\u003A" },
			{ U"semi", U"\u003B" },
			{ U"lt", U"\u003C" },
			{ U"LT", U"\u003C" },
			{ U"equals", U"\u003D" },
			{ U"gt", U"\u003E" },
			{ U"GT", U"\u003E" },
			{ U"quest", U"\u003F" },
			{ U"commat", U"\u0040" },
			{ U"lsqb", U"\u005B" },
			{ U"lbrack", U"\u005B" },
			{ U"bsol", U"\u005C" },
			{ U"rsqb", U"\u005D" },
			{ U"rbrack", U"\u005D" },
			{ U"Hat", U"\u005E" },
			{ U"lowbar", U"\u005F" },
			{ U"UnderBar", U"\u005F" },
			{ U"grave", U"\u0060" },
			{ U"DiacriticalGrave", U"\u0060" },
			{ U"lcub", U"\u007B" },
			{ U"lbrace", U"\u007B" },
			{ U"verbar", U"\u007C" },
			{ U"vert", U"\u007C" },
			{ U"VerticalLine", U"\u007C" },
			{ U"rcub", U"\u007D" },
			{ U"rbrace", U"\u007D" },
			{ U"nbsp", U" " },
			{ U"NonBreakingSpace", U"\u00A0" },
			{ U"iexcl", U"\u00A1" },
			{ U"cent", U"\u00A2" },
			{ U"pound", U"\u00A3" },
			{ U"curren", U"\u00A4" },
			{ U"yen", U"\u00A5" },
			{ U"brvbar", U"\u00A6" },
			{ U"sect", U"\u00A7" },
			{ U"Dot", U"\u00A8" },
			{ U"die", U"\u00A8" },
			{ U"DoubleDot", U"\u00A8" },
			{ U"uml", U"\u00A8" },
			{ U"copy", U"\u00A9" },
			{ U"COPY", U"\u00A9" },
			{ U"ordf", U"\u00AA" },
			{ U"laquo", U"\u00AB" },
			{ U"not", U"\u00AC" },
			{ U"shy", U"\u00AD" },
			{ U"reg", U"\u00AE" },
			{ U"circledR", U"\u00AE" },
			{ U"REG", U"\u00AE" },
			{ U"macr", U"\u00AF" },
			{ U"strns", U"\u00AF" },
			{ U"deg", U"\u00B0" },
			{ U"plusmn", U"\u00B1" },
			{ U"pm", U"\u00B1" },
			{ U"PlusMinus", U"\u00B1" },
			{ U"sup2", U"\u00B2" },
			{ U"sup3", U"\u00B3" },
			{ U"acute", U"\u00B4" },
			{ U"DiacriticalAcute", U"\u00B4" },
			{ U"micro", U"\u00B5" },
			{ U"para", U"\u00B6" },
			{ U"middot", U"\u00B7" },
			{ U"centerdot", U"\u00B7" },
			{ U"CenterDot", U"\u00B7" },
			{ U"cedil", U"\u00B8" },
			{ U"Cedilla", U"\u00B8" },
			{ U"sup1", U"\u00B9" },
			{ U"ordm", U"\u00BA" },
			{ U"raquo", U"\u00BB" },
			{ U"frac14", U"\u00BC" },
			{ U"frac12", U"\u00BD" },
			{ U"half", U"\u00BD" },
			{ U"frac34", U"\u00BE" },
			{ U"iquest", U"\u00BF" },
			{ U"Agrave", U"\u00C0" },
			{ U"Aacute", U"\u00C1" },
			{ U"Acirc", U"\u00C2" },
			{ U"Atilde", U"\u00C3" },
			{ U"Auml", U"\u00C4" },
			{ U"Aring", U"\u00C5" },
			{ U"angst", U"\u00C5" },
			{ U"AElig", U"\u00C6" },
			{ U"Ccedil", U"\u00C7" },
			{ U"Egrave", U"\u00C8" },
			{ U"Eacute", U"\u00C9" },
			{ U"Ecirc", U"\u00CA" },
			{ U"Euml", U"\u00CB" },
			{ U"Igrave", U"\u00CC" },
			{ U"Iacute", U"\u00CD" },
			{ U"Icirc", U"\u00CE" },
			{ U"Iuml", U"\u00CF" },
			{ U"ETH", U"\u00D0" },
			{ U"Ntilde", U"\u00D1" },
			{ U"Ograve", U"\u00D2" },
			{ U"Oacute", U"\u00D3" },
			{ U"Ocirc", U"\u00D4" },
			{ U"Otilde", U"\u00D5" },
			{ U"Ouml", U"\u00D6" },
			{ U"times", U"\u00D7" },
			{ U"Oslash", U"\u00D8" },
			{ U"Ugrave", U"\u00D9" },
			{ U"Uacute", U"\u00DA" },
			{ U"Ucirc", U"\u00DB" },
			{ U"Uuml", U"\u00DC" },
			{ U"Yacute", U"\u00DD" },
			{ U"THORN", U"\u00DE" },
			{ U"szlig", U"\u00DF" },
			{ U"agrave", U"\u00E0" },
			{ U"aacute", U"\u00E1" },
			{ U"acirc", U"\u00E2" },
			{ U"atilde", U"\u00E3" },
			{ U"auml", U"\u00E4" },
			{ U"aring", U"\u00E5" },
			{ U"aelig", U"\u00E6" },
			{ U"ccedil", U"\u00E7" },
			{ U"egrave", U"\u00E8" },
			{ U"eacute", U"\u00E9" },
			{ U"ecirc", U"\u00EA" },
			{ U"euml", U"\u00EB" },
			{ U"igrave", U"\u00EC" },
			{ U"iacute", U"\u00ED" },
			{ U"icirc", U"\u00EE" },
			{ U"iuml", U"\u00EF" },
			{ U"eth", U"\u00F0" },
			{ U"ntilde", U"\u00F1" },
			{ U"ograve", U"\u00F2" },
			{ U"oacute", U"\u00F3" },
			{ U"ocirc", U"\u00F4" },
			{ U"otilde", U"\u00F5" },
			{ U"ouml", U"\u00F6" },
			{ U"divide", U"\u00F7" },
			{ U"div", U"\u00F7" },
			{ U"oslash", U"\u00F8" },
			{ U"ugrave", U"\u00F9" },
			{ U"uacute", U"\u00FA" },
			{ U"ucirc", U"\u00FB" },
			{ U"uuml", U"\u00FC" },
			{ U"yacute", U"\u00FD" },
			{ U"thorn", U"\u00FE" },
			{ U"yuml", U"\u00FF" },
			{ U"Amacr", U"\u0100" },
			{ U"amacr", U"\u0101" },
			{ U"Abreve", U"\u0102" },
			{ U"abreve", U"\u0103" },
			{ U"Aogon", U"\u0104" },
			{ U"aogon", U"\u0105" },
			{ U"Cacute", U"\u0106" },
			{ U"cacute", U"\u0107" },
			{ U"Ccirc", U"\u0108" },
			{ U"ccirc", U"\u0109" },
			{ U"Cdot", U"\u010A" },
			{ U"cdot", U"\u010B" },
			{ U"Ccaron", U"\u010C" },
			{ U"ccaron", U"\u010D" },
			{ U"Dcaron", U"\u010E" },
			{ U"dcaron", U"\u010F" },
			{ U"Dstrok", U"\u0110" },
			{ U"dstrok", U"\u0111" },
			{ U"Emacr", U"\u0112" },
			{ U"emacr", U"\u0113" },
			{ U"Edot", U"\u0116" },
			{ U"edot", U"\u0117" },
			{ U"Eogon", U"\u0118" },
			{ U"eogon", U"\u0119" },
			{ U"Ecaron", U"\u011A" },
			{ U"ecaron", U"\u011B" },
			{ U"Gcirc", U"\u011C" },
			{ U"gcirc", U"\u011D" },
			{ U"Gbreve", U"\u011E" },
			{ U"gbreve", U"\u011F" },
			{ U"Gdot", U"\u0120" },
			{ U"gdot", U"\u0121" },
			{ U"Gcedil", U"\u0122" },
			{ U"Hcirc", U"\u0124" },
			{ U"hcirc", U"\u0125" },
			{ U"Hstrok", U"\u0126" },
			{ U"hstrok", U"\u0127" },
			{ U"Itilde", U"\u0128" },
			{ U"itilde", U"\u0129" },
			{ U"Imacr", U"\u012A" },
			{ U"imacr", U"\u012B" },
			{ U"Iogon", U"\u012E" },
			{ U"iogon", U"\u012F" },
			{ U"Idot", U"\u0130" },
			{ U"imath", U"\u0131" },
			{ U"inodot", U"\u0131" },
			{ U"IJlig", U"\u0132" },
			{ U"ijlig", U"\u0133" },
			{ U"Jcirc", U"\u0134" },
			{ U"jcirc", U"\u0135" },
			{ U"Kcedil", U"\u0136" },
			{ U"kcedil", U"\u0137" },
			{ U"kgreen", U"\u0138" },
			{ U"Lacute", U"\u0139" },
			{ U"lacute", U"\u013A" },
			{ U"Lcedil", U"\u013B" },
			{ U"lcedil", U"\u013C" },
			{ U"Lcaron", U"\u013D" },
			{ U"lcaron", U"\u013E" },
			{ U"Lmidot", U"\u013F" },
			{ U"lmidot", U"\u0140" },
			{ U"Lstrok", U"\u0141" },
			{ U"lstrok", U"\u0142" },
			{ U"Nacute", U"\u0143" },
			{ U"nacute", U"\u0144" },
			{ U"Ncedil", U"\u0145" },
			{ U"ncedil", U"\u0146" },
			{ U"Ncaron", U"\u0147" },
			{ U"ncaron", U"\u0148" },
			{ U"napos", U"\u0149" },
			{ U"ENG", U"\u014A" },
			{ U"eng", U"\u014B" },
			{ U"Omacr", U"\u014C" },
			{ U"omacr", U"\u014D" },
			{ U"Odblac", U"\u0150" },
			{ U"odblac", U"\u0151" },
			{ U"OElig", U"\u0152" },
			{ U"oelig", U"\u0153" },
			{ U"Racute", U"\u0154" },
			{ U"racute", U"\u0155" },
			{ U"Rcedil", U"\u0156" },
			{ U"rcedil", U"\u0157" },
			{ U"Rcaron", U"\u0158" },
			{ U"rcaron", U"\u0159" },
			{ U"Sacute", U"\u015A" },
			{ U"sacute", U"\u015B" },
			{ U"Scirc", U"\u015C" },
			{ U"scirc", U"\u015D" },
			{ U"Scedil", U"\u015E" },
			{ U"scedil", U"\u015F" },
			{ U"Scaron", U"\u0160" },
			{ U"scaron", U"\u0161" },
			{ U"Tcedil", U"\u0162" },
			{ U"tcedil", U"\u0163" },
			{ U"Tcaron", U"\u0164" },
			{ U"tcaron", U"\u0165" },
			{ U"Tstrok", U"\u0166" },
			{ U"tstrok", U"\u0167" },
			{ U"Utilde", U"\u0168" },
			{ U"utilde", U"\u0169" },
			{ U"Umacr", U"\u016A" },
			{ U"umacr", U"\u016B" },
			{ U"Ubreve", U"\u016C" },
			{ U"ubreve", U"\u016D" },
			{ U"Uring", U"\u016E" },
			{ U"uring", U"\u016F" },
			{ U"Udblac", U"\u0170" },
			{ U"udblac", U"\u0171" },
			{ U"Uogon", U"\u0172" },
			{ U"uogon", U"\u0173" },
			{ U"Wcirc", U"\u0174" },
			{ U"wcirc", U"\u0175" },
			{ U"Ycirc", U"\u0176" },
			{ U"ycirc", U"\u0177" },
			{ U"Yuml", U"\u0178" },
			{ U"Zacute", U"\u0179" },
			{ U"zacute", U"\u017A" },
			{ U"Zdot", U"\u017B" },
			{ U"zdot", U"\u017C" },
			{ U"Zcaron", U"\u017D" },
			{ U"zcaron", U"\u017E" },
			{ U"fnof", U"\u0192" },
			{ U"imped", U"\u01B5" },
			{ U"gacute", U"\u01F5" },
			{ U"jmath", U"\u0237" },
			{ U"circ", U"\u02C6" },
			{ U"Hacek", U"\u02C7" },
			{ U"caron", U"\u02C7" },
			{ U"breve", U"\u02D8" },
			{ U"Breve", U"\u02D8" },
			{ U"dot", U"\u02D9" },
			{ U"DiacriticalDot", U"\u02D9" },
			{ U"ring", U"\u02DA" },
			{ U"ogon", U"\u02DB" },
			{ U"tilde", U"\u02DC" },
			{ U"DiacriticalTilde", U"\u02DC" },
			{ U"dblac", U"\u02DD" },
			{ U"DiacriticalDoubleAcute", U"\u02DD" },
			{ U"DownBreve", U"\u0311" },
			{ U"Alpha", U"\u0391" },
			{ U"Beta", U"\u0392" },
			{ U"Gamma", U"\u0393" },
			{ U"Delta", U"\u0394" },
			{ U"Epsilon", U"\u0395" },
			{ U"Zeta", U"\u0396" },
			{ U"Eta", U"\u0397" },
			{ U"Theta", U"\u0398" },
			{ U"Iota", U"\u0399" },
			{ U"Kappa", U"\u039A" },
			{ U"Lambda", U"\u039B" },
			{ U"Mu", U"\u039C" },
			{ U"Nu", U"\u039D" },
			{ U"Xi", U"\u039E" },
			{ U"Omicron", U"\u039F" },
			{ U"Pi", U"\u03A0" },
			{ U"Rho", U"\u03A1" },
			{ U"Sigma", U"\u03A3" },
			{ U"Tau", U"\u03A4" },
			{ U"Upsilon", U"\u03A5" },
			{ U"Phi", U"\u03A6" },
			{ U"Chi", U"\u03A7" },
			{ U"Psi", U"\u03A8" },
			{ U"Omega", U"\u03A9" },
			{ U"ohm", U"\u03A9" },
			{ U"alpha", U"\u03B1" },
			{ U"beta", U"\u03B2" },
			{ U"gamma", U"\u03B3" },
			{ U"delta", U"\u03B4" },
			{ U"epsi", U"\u03B5" },
			{ U"epsilon", U"\u03B5" },
			{ U"zeta", U"\u03B6" },
			{ U"eta", U"\u03B7" },
			{ U"theta", U"\u03B8" },
			{ U"iota", U"\u03B9" },
			{ U"kappa", U"\u03BA" },
			{ U"lambda", U"\u03BB" },
			{ U"mu", U"\u03BC" },
			{ U"nu", U"\u03BD" },
			{ U"xi", U"\u03BE" },
			{ U"omicron", U"\u03BF" },
			{ U"pi", U"\u03C0" },
			{ U"rho", U"\u03C1" },
			{ U"sigmav", U"\u03C2" },
			{ U"varsigma", U"\u03C2" },
			{ U"sigmaf", U"\u03C2" },
			{ U"sigma", U"\u03C3" },
			{ U"tau", U"\u03C4" },
			{ U"upsi", U"\u03C5" },
			{ U"upsilon", U"\u03C5" },
			{ U"phi", U"\u03C6" },
			{ U"chi", U"\u03C7" },
			{ U"psi", U"\u03C8" },
			{ U"omega", U"\u03C9" },
			{ U"thetav", U"\u03D1" },
			{ U"vartheta", U"\u03D1" },
			{ U"thetasym", U"\u03D1" },
			{ U"Upsi", U"\u03D2" },
			{ U"upsih", U"\u03D2" },
			{ U"straightphi", U"\u03D5" },
			{ U"phiv", U"\u03D5" },
			{ U"varphi", U"\u03D5" },
			{ U"piv", U"\u03D6" },
			{ U"varpi", U"\u03D6" },
			{ U"Gammad", U"\u03DC" },
			{ U"gammad", U"\u03DD" },
			{ U"digamma", U"\u03DD" },
			{ U"kappav", U"\u03F0" },
			{ U"varkappa", U"\u03F0" },
			{ U"rhov", U"\u03F1" },
			{ U"varrho", U"\u03F1" },
			{ U"epsiv", U"\u03F5" },
			{ U"varepsilon", U"\u03F5" },
			{ U"straightepsilon", U"\u03F5" },
			{ U"bepsi", U"\u03F6" },
			{ U"backepsilon", U"\u03F6" },
			{ U"IOcy", U"\u0401" },
			{ U"DJcy", U"\u0402" },
			{ U"GJcy", U"\u0403" },
			{ U"Jukcy", U"\u0404" },
			{ U"DScy", U"\u0405" },
			{ U"Iukcy", U"\u0406" },
			{ U"YIcy", U"\u0407" },
			{ U"Jsercy", U"\u0408" },
			{ U"LJcy", U"\u0409" },
			{ U"NJcy", U"\u040A" },
			{ U"TSHcy", U"\u040B" },
			{ U"KJcy", U"\u040C" },
			{ U"Ubrcy", U"\u040E" },
			{ U"DZcy", U"\u040F" },
			{ U"Acy", U"\u0410" },
			{ U"Bcy", U"\u0411" },
			{ U"Vcy", U"\u0412" },
			{ U"Gcy", U"\u0413" },
			{ U"Dcy", U"\u0414" },
			{ U"IEcy", U"\u0415" },
			{ U"ZHcy", U"\u0416" },
			{ U"Zcy", U"\u0417" },
			{ U"Icy", U"\u0418" },
			{ U"Jcy", U"\u0419" },
			{ U"Kcy", U"\u041A" },
			{ U"Lcy", U"\u041B" },
			{ U"Mcy", U"\u041C" },
			{ U"Ncy", U"\u041D" },
			{ U"Ocy", U"\u041E" },
			{ U"Pcy", U"\u041F" },
			{ U"Rcy", U"\u0420" },
			{ U"Scy", U"\u0421" },
			{ U"Tcy", U"\u0422" },
			{ U"Ucy", U"\u0423" },
			{ U"Fcy", U"\u0424" },
			{ U"KHcy", U"\u0425" },
			{ U"TScy", U"\u0426" },
			{ U"CHcy", U"\u0427" },
			{ U"SHcy", U"\u0428" },
			{ U"SHCHcy", U"\u0429" },
			{ U"HARDcy", U"\u042A" },
			{ U"Ycy", U"\u042B" },
			{ U"SOFTcy", U"\u042C" },
			{ U"Ecy", U"\u042D" },
			{ U"YUcy", U"\u042E" },
			{ U"YAcy", U"\u042F" },
			{ U"acy", U"\u0430" },
			{ U"bcy", U"\u0431" },
			{ U"vcy", U"\u0432" },
			{ U"gcy", U"\u0433" },
			{ U"dcy", U"\u0434" },
			{ U"iecy", U"\u0435" },
			{ U"zhcy", U"\u0436" },
			{ U"zcy", U"\u0437" },
			{ U"icy", U"\u0438" },
			{ U"jcy", U"\u0439" },
			{ U"kcy", U"\u043A" },
			{ U"lcy", U"\u043B" },
			{ U"mcy", U"\u043C" },
			{ U"ncy", U"\u043D" },
			{ U"ocy", U"\u043E" },
			{ U"pcy", U"\u043F" },
			{ U"rcy", U"\u0440" },
			{ U"scy", U"\u0441" },
			{ U"tcy", U"\u0442" },
			{ U"ucy", U"\u0443" },
			{ U"fcy", U"\u0444" },
			{ U"khcy", U"\u0445" },
			{ U"tscy", U"\u0446" },
			{ U"chcy", U"\u0447" },
			{ U"shcy", U"\u0448" },
			{ U"shchcy", U"\u0449" },
			{ U"hardcy", U"\u044A" },
			{ U"ycy", U"\u044B" },
			{ U"softcy", U"\u044C" },
			{ U"ecy", U"\u044D" },
			{ U"yucy", U"\u044E" },
			{ U"yacy", U"\u044F" },
			{ U"iocy", U"\u0451" },
			{ U"djcy", U"\u0452" },
			{ U"gjcy", U"\u0453" },
			{ U"jukcy", U"\u0454" },
			{ U"dscy", U"\u0455" },
			{ U"iukcy", U"\u0456" },
			{ U"yicy", U"\u0457" },
			{ U"jsercy", U"\u0458" },
			{ U"ljcy", U"\u0459" },
			{ U"njcy", U"\u045A" },
			{ U"tshcy", U"\u045B" },
			{ U"kjcy", U"\u045C" },
			{ U"ubrcy", U"\u045E" },
			{ U"dzcy", U"\u045F" },
			{ U"ensp", U"\u2002" },
			{ U"emsp", U"\u2003" },
			{ U"emsp13", U"\u2004" },
			{ U"emsp14", U"\u2005" },
			{ U"numsp", U"\u2007" },
			{ U"puncsp", U"\u2008" },
			{ U"thinsp", U"\u2009" },
			{ U"ThinSpace", U"\u2009" },
			{ U"hairsp", U"\u200A" },
			{ U"VeryThinSpace", U"\u200A" },
			{ U"ZeroWidthSpace", U"\u200B" },
			{ U"NegativeVeryThinSpace", U"\u200B" },
			{ U"NegativeThinSpace", U"\u200B" },
			{ U"NegativeMediumSpace", U"\u200B" },
			{ U"NegativeThickSpace", U"\u200B" },
			{ U"zwnj", U"\u200C" },
			{ U"zwj", U"\u200D" },
			{ U"lrm", U"\u200E" },
			{ U"rlm", U"\u200F" },
			{ U"hyphen", U"\u2010" },
			{ U"dash", U"\u2010" },
			{ U"ndash", U"\u2013" },
			{ U"mdash", U"\u2014" },
			{ U"horbar", U"\u2015" },
			{ U"Verbar", U"\u2016" },
			{ U"Vert", U"\u2016" },
			{ U"lsquo", U"\u2018" },
			{ U"OpenCurlyQuote", U"\u2018" },
			{ U"rsquo", U"\u2019" },
			{ U"rsquor", U"\u2019" },
			{ U"CloseCurlyQuote", U"\u2019" },
			{ U"sbquo", U"\u201A" },
			{ U"lsquor", U"\u201A" },
			{ U"ldquo", U"\u201C" },
			{ U"OpenCurlyDoubleQuote", U"\u201C" },
			{ U"rdquo", U"\u201D" },
			{ U"rdquor", U"\u201D" },
			{ U"CloseCurlyDoubleQuote", U"\u201D" },
			{ U"bdquo", U"\u201E" },
			{ U"ldquor", U"\u201E" },
			{ U"dagger", U"\u2020" },
			{ U"Dagger", U"\u2021" },
			{ U"ddagger", U"\u2021" },
			{ U"bull", U"\u2022" },
			{ U"bullet", U"\u2022" },
			{ U"nldr", U"\u2025" },
			{ U"hellip", U"\u2026" },
			{ U"mldr", U"\u2026" },
			{ U"permil", U"\u2030" },
			{ U"pertenk", U"\u2031" },
			{ U"prime", U"\u2032" },
			{ U"Prime", U"\u2033" },
			{ U"tprime", U"\u2034" },
			{ U"bprime", U"\u2035" },
			{ U"backprime", U"\u2035" },
			{ U"lsaquo", U"\u2039" },
			{ U"rsaquo", U"\u203A" },
			{ U"oline", U"\u203E" },
			{ U"OverBar", U"\u203E" },
			{ U"caret", U"\u2041" },
			{ U"hybull", U"\u2043" },
			{ U"frasl", U"\u2044" },
			{ U"bsemi", U"\u204F" },
			{ U"qprime", U"\u2057" },
			{ U"MediumSpace", U"\u205F" },
			{ U"NoBreak", U"\u2060" },
			{ U"ApplyFunction", U"\u2061" },
			{ U"af", U"\u2061" },
			{ U"InvisibleTimes", U"\u2062" },
			{ U"it", U"\u2062" },
			{ U"InvisibleComma", U"\u2063" },
			{ U"ic", U"\u2063" },
			{ U"euro", U"\u20AC" },
			{ U"tdot", U"\u20DB" },
			{ U"TripleDot", U"\u20DB" },
			{ U"DotDot", U"\u20DC" },
			{ U"Copf", U"\u2102" },
			{ U"complexes", U"\u2102" },
			{ U"incare", U"\u2105" },
			{ U"gscr", U"\u210A" },
			{ U"hamilt", U"\u210B" },
			{ U"HilbertSpace", U"\u210B" },
			{ U"Hscr", U"\u210B" },
			{ U"Hfr", U"\u210C" },
			{ U"Poincareplane", U"\u210C" },
			{ U"quaternions", U"\u210D" },
			{ U"Hopf", U"\u210D" },
			{ U"planckh", U"\u210E" },
			{ U"planck", U"\u210F" },
			{ U"hbar", U"\u210F" },
			{ U"plankv", U"\u210F" },
			{ U"hslash", U"\u210F" },
			{ U"Iscr", U"\u2110" },
			{ U"imagline", U"\u2110" },
			{ U"image", U"\u2111" },
			{ U"Im", U"\u2111" },
			{ U"imagpart", U"\u2111" },
			{ U"Ifr", U"\u2111" },
			{ U"Lscr", U"\u2112" },
			{ U"lagran", U"\u2112" },
			{ U"Laplacetrf", U"\u2112" },
			{ U"ell", U"\u2113" },
			{ U"Nopf", U"\u2115" },
			{ U"naturals", U"\u2115" },
			{ U"numero", U"\u2116" },
			{ U"copysr", U"\u2117" },
			{ U"weierp", U"\u2118" },
			{ U"wp", U"\u2118" },
			{ U"Popf", U"\u2119" },
			{ U"primes", U"\u2119" },
			{ U"rationals", U"\u211A" },
			{ U"Qopf", U"\u211A" },
			{ U"Rscr", U"\u211B" },
			{ U"realine", U"\u211B" },
			{ U"real", U"\u211C" },
			{ U"Re", U"\u211C" },
			{ U"realpart", U"\u211C" },
			{ U"Rfr", U"\u211C" },
			{ U"reals", U"\u211D" },
			{ U"Ropf", U"\u211D" },
			{ U"rx", U"\u211E" },
			{ U"trade", U"\u2122" },
			{ U"TRADE", U"\u2122" },
			{ U"integers", U"\u2124" },
			{ U"Zopf", U"\u2124" },
			{ U"mho", U"\u2127" },
			{ U"Zfr", U"\u2128" },
			{ U"zeetrf", U"\u2128" },
			{ U"iiota", U"\u2129" },
			{ U"bernou", U"\u212C" },
			{ U"Bernoullis", U"\u212C" },
			{ U"Bscr", U"\u212C" },
			{ U"Cfr", U"\u212D" },
			{ U"Cayleys", U"\u212D" },
			{ U"escr", U"\u212F" },
			{ U"Escr", U"\u2130" },
			{ U"expectation", U"\u2130" },
			{ U"Fscr", U"\u2131" },
			{ U"Fouriertrf", U"\u2131" },
			{ U"phmmat", U"\u2133" },
			{ U"Mellintrf", U"\u2133" },
			{ U"Mscr", U"\u2133" },
			{ U"order", U"\u2134" },
			{ U"orderof", U"\u2134" },
			{ U"oscr", U"\u2134" },
			{ U"alefsym", U"\u2135" },
			{ U"aleph", U"\u2135" },
			{ U"beth", U"\u2136" },
			{ U"gimel", U"\u2137" },
			{ U"daleth", U"\u2138" },
			{ U"CapitalDifferentialD", U"\u2145" },
			{ U"DD", U"\u2145" },
			{ U"DifferentialD", U"\u2146" },
			{ U"dd", U"\u2146" },
			{ U"ExponentialE", U"\u2147" },
			{ U"exponentiale", U"\u2147" },
			{ U"ee", U"\u2147" },
			{ U"ImaginaryI", U"\u2148" },
			{ U"ii", U"\u2148" },
			{ U"frac13", U"\u2153" },
			{ U"frac23", U"\u2154" },
			{ U"frac15", U"\u2155" },
			{ U"frac25", U"\u2156" },
			{ U"frac35", U"\u2157" },
			{ U"frac45", U"\u2158" },
			{ U"frac16", U"\u2159" },
			{ U"frac56", U"\u215A" },
			{ U"frac18", U"\u215B" },
			{ U"frac38", U"\u215C" },
			{ U"frac58", U"\u215D" },
			{ U"frac78", U"\u215E" },
			{ U"larr", U"\u2190" },
			{ U"leftarrow", U"\u2190" },
			{ U"LeftArrow", U"\u2190" },
			{ U"slarr", U"\u2190" },
			{ U"ShortLeftArrow", U"\u2190" },
			{ U"uarr", U"\u2191" },
			{ U"uparrow", U"\u2191" },
			{ U"UpArrow", U"\u2191" },
			{ U"ShortUpArrow", U"\u2191" },
			{ U"rarr", U"\u2192" },
			{ U"rightarrow", U"\u2192" },
			{ U"RightArrow", U"\u2192" },
			{ U"srarr", U"\u2192" },
			{ U"ShortRightArrow", U"\u2192" },
			{ U"darr", U"\u2193" },
			{ U"downarrow", U"\u2193" },
			{ U"DownArrow", U"\u2193" },
			{ U"ShortDownArrow", U"\u2193" },
			{ U"harr", U"\u2194" },
			{ U"leftrightarrow", U"\u2194" },
			{ U"LeftRightArrow", U"\u2194" },
			{ U"varr", U"\u2195" },
			{ U"updownarrow", U"\u2195" },
			{ U"UpDownArrow", U"\u2195" },
			{ U"nwarr", U"\u2196" },
			{ U"UpperLeftArrow", U"\u2196" },
			{ U"nwarrow", U"\u2196" },
			{ U"nearr", U"\u2197" },
			{ U"UpperRightArrow", U"\u2197" },
			{ U"nearrow", U"\u2197" },
			{ U"searr", U"\u2198" },
			{ U"searrow", U"\u2198" },
			{ U"LowerRightArrow", U"\u2198" },
			{ U"swarr", U"\u2199" },
			{ U"swarrow", U"\u2199" },
			{ U"LowerLeftArrow", U"\u2199" },
			{ U"nlarr", U"\u219A" },
			{ U"nleftarrow", U"\u219A" },
			{ U"nrarr", U"\u219B" },
			{ U"nrightarrow", U"\u219B" },
			{ U"rarrw", U"\u219D" },
			{ U"rightsquigarrow", U"\u219D" },
			{ U"Larr", U"\u219E" },
			{ U"twoheadleftarrow", U"\u219E" },
			{ U"Uarr", U"\u219F" },
			{ U"Rarr", U"\u21A0" },
			{ U"twoheadrightarrow", U"\u21A0" },
			{ U"Darr", U"\u21A1" },
			{ U"larrtl", U"\u21A2" },
			{ U"leftarrowtail", U"\u21A2" },
			{ U"rarrtl", U"\u21A3" },
			{ U"rightarrowtail", U"\u21A3" },
			{ U"LeftTeeArrow", U"\u21A4" },
			{ U"mapstoleft", U"\u21A4" },
			{ U"UpTeeArrow", U"\u21A5" },
			{ U"mapstoup", U"\u21A5" },
			{ U"map", U"\u21A6" },
			{ U"RightTeeArrow", U"\u21A6" },
			{ U"mapsto", U"\u21A6" },
			{ U"DownTeeArrow", U"\u21A7" },
			{ U"mapstodown", U"\u21A7" },
			{ U"larrhk", U"\u21A9" },
			{ U"hookleftarrow", U"\u21A9" },
			{ U"rarrhk", U"\u21AA" },
			{ U"hookrightarrow", U"\u21AA" },
			{ U"larrlp", U"\u21AB" },
			{ U"looparrowleft", U"\u21AB" },
			{ U"rarrlp", U"\u21AC" },
			{ U"looparrowright", U"\u21AC" },
			{ U"harrw", U"\u21AD" },
			{ U"leftrightsquigarrow", U"\u21AD" },
			{ U"nharr", U"\u21AE" },
			{ U"nleftrightarrow", U"\u21AE" },
			{ U"lsh", U"\u21B0" },
			{ U"Lsh", U"\u21B0" },
			{ U"rsh", U"\u21B1" },
			{ U"Rsh", U"\u21B1" },
			{ U"ldsh", U"\u21B2" },
			{ U"rdsh", U"\u21B3" },
			{ U"crarr", U"\u21B5" },
			{ U"cularr", U"\u21B6" },
			{ U"curvearrowleft", U"\u21B6" },
			{ U"curarr", U"\u21B7" },
			{ U"curvearrowright", U"\u21B7" },
			{ U"olarr", U"\u21BA" },
			{ U"circlearrowleft", U"\u21BA" },
			{ U"orarr", U"\u21BB" },
			{ U"circlearrowright", U"\u21BB" },
			{ U"lharu", U"\u21BC" },
			{ U"LeftVector", U"\u21BC" },
			{ U"leftharpoonup", U"\u21BC" },
			{ U"lhard", U"\u21BD" },
			{ U"leftharpoondown", U"\u21BD" },
			{ U"DownLeftVector", U"\u21BD" },
			{ U"uharr", U"\u21BE" },
			{ U"upharpoonright", U"\u21BE" },
			{ U"RightUpVector", U"\u21BE" },
			{ U"uharl", U"\u21BF" },
			{ U"upharpoonleft", U"\u21BF" },
			{ U"LeftUpVector", U"\u21BF" },
			{ U"rharu", U"\u21C0" },
			{ U"RightVector", U"\u21C0" },
			{ U"rightharpoonup", U"\u21C0" },
			{ U"rhard", U"\u21C1" },
			{ U"rightharpoondown", U"\u21C1" },
			{ U"DownRightVector", U"\u21C1" },
			{ U"dharr", U"\u21C2" },
			{ U"RightDownVector", U"\u21C2" },
			{ U"downharpoonright", U"\u21C2" },
			{ U"dharl", U"\u21C3" },
			{ U"LeftDownVector", U"\u21C3" },
			{ U"downharpoonleft", U"\u21C3" },
			{ U"rlarr", U"\u21C4" },
			{ U"rightleftarrows", U"\u21C4" },
			{ U"RightArrowLeftArrow", U"\u21C4" },
			{ U"udarr", U"\u21C5" },
			{ U"UpArrowDownArrow", U"\u21C5" },
			{ U"lrarr", U"\u21C6" },
			{ U"leftrightarrows", U"\u21C6" },
			{ U"LeftArrowRightArrow", U"\u21C6" },
			{ U"llarr", U"\u21C7" },
			{ U"leftleftarrows", U"\u21C7" },
			{ U"uuarr", U"\u21C8" },
			{ U"upuparrows", U"\u21C8" },
			{ U"rrarr", U"\u21C9" },
			{ U"rightrightarrows", U"\u21C9" },
			{ U"ddarr", U"\u21CA" },
			{ U"downdownarrows", U"\u21CA" },
			{ U"lrhar", U"\u21CB" },
			{ U"ReverseEquilibrium", U"\u21CB" },
			{ U"leftrightharpoons", U"\u21CB" },
			{ U"rlhar", U"\u21CC" },
			{ U"rightleftharpoons", U"\u21CC" },
			{ U"Equilibrium", U"\u21CC" },
			{ U"nlArr", U"\u21CD" },
			{ U"nLeftarrow", U"\u21CD" },
			{ U"nhArr", U"\u21CE" },
			{ U"nLeftrightarrow", U"\u21CE" },
			{ U"nrArr", U"\u21CF" },
			{ U"nRightarrow", U"\u21CF" },
			{ U"lArr", U"\u21D0" },
			{ U"Leftarrow", U"\u21D0" },
			{ U"DoubleLeftArrow", U"\u21D0" },
			{ U"uArr", U"\u21D1" },
			{ U"Uparrow", U"\u21D1" },
			{ U"DoubleUpArrow", U"\u21D1" },
			{ U"rArr", U"\u21D2" },
			{ U"Rightarrow", U"\u21D2" },
			{ U"Implies", U"\u21D2" },
			{ U"DoubleRightArrow", U"\u21D2" },
			{ U"dArr", U"\u21D3" },
			{ U"Downarrow", U"\u21D3" },
			{ U"DoubleDownArrow", U"\u21D3" },
			{ U"hArr", U"\u21D4" },
			{ U"Leftrightarrow", U"\u21D4" },
			{ U"DoubleLeftRightArrow", U"\u21D4" },
			{ U"iff", U"\u21D4" },
			{ U"vArr", U"\u21D5" },
			{ U"Updownarrow", U"\u21D5" },
			{ U"DoubleUpDownArrow", U"\u21D5" },
			{ U"nwArr", U"\u21D6" },
			{ U"neArr", U"\u21D7" },
			{ U"seArr", U"\u21D8" },
			{ U"swArr", U"\u21D9" },
			{ U"lAarr", U"\u21DA" },
			{ U"Lleftarrow", U"\u21DA" },
			{ U"rAarr", U"\u21DB" },
			{ U"Rrightarrow", U"\u21DB" },
			{ U"zigrarr", U"\u21DD" },
			{ U"larrb", U"\u21E4" },
			{ U"LeftArrowBar", U"\u21E4" },
			{ U"rarrb", U"\u21E5" },
			{ U"RightArrowBar", U"\u21E5" },
			{ U"duarr", U"\u21F5" },
			{ U"DownArrowUpArrow", U"\u21F5" },
			{ U"loarr", U"\u21FD" },
			{ U"roarr", U"\u21FE" },
			{ U"hoarr", U"\u21FF" },
			{ U"forall", U"\u2200" },
			{ U"ForAll", U"\u2200" },
			{ U"comp", U"\u2201" },
			{ U"complement", U"\u2201" },
			{ U"part", U"\u2202" },
			{ U"PartialD", U"\u2202" },
			{ U"exist", U"\u2203" },
			{ U"Exists", U"\u2203" },
			{ U"nexist", U"\u2204" },
			{ U"NotExists", U"\u2204" },
			{ U"nexists", U"\u2204" },
			{ U"empty", U"\u2205" },
			{ U"emptyset", U"\u2205" },
			{ U"emptyv", U"\u2205" },
			{ U"varnothing", U"\u2205" },
			{ U"nabla", U"\u2207" },
			{ U"Del", U"\u2207" },
			{ U"isin", U"\u2208" },
			{ U"isinv", U"\u2208" },
			{ U"Element", U"\u2208" },
			{ U"in", U"\u2208" },
			{ U"notin", U"\u2209" },
			{ U"NotElement", U"\u2209" },
			{ U"notinva", U"\u2209" },
			{ U"niv", U"\u220B" },
			{ U"ReverseElement", U"\u220B" },
			{ U"ni", U"\u220B" },
			{ U"SuchThat", U"\u220B" },
			{ U"notni", U"\u220C" },
			{ U"notniva", U"\u220C" },
			{ U"NotReverseElement", U"\u220C" },
			{ U"prod", U"\u220F" },
			{ U"Product", U"\u220F" },
			{ U"coprod", U"\u2210" },
			{ U"Coproduct", U"\u2210" },
			{ U"sum", U"\u2211" },
			{ U"Sum", U"\u2211" },
			{ U"minus", U"\u2212" },
			{ U"mnplus", U"\u2213" },
			{ U"mp", U"\u2213" },
			{ U"MinusPlus", U"\u2213" },
			{ U"plusdo", U"\u2214" },
			{ U"dotplus", U"\u2214" },
			{ U"setmn", U"\u2216" },
			{ U"setminus", U"\u2216" },
			{ U"Backslash", U"\u2216" },
			{ U"ssetmn", U"\u2216" },
			{ U"smallsetminus", U"\u2216" },
			{ U"lowast", U"\u2217" },
			{ U"compfn", U"\u2218" },
			{ U"SmallCircle", U"\u2218" },
			{ U"radic", U"\u221A" },
			{ U"Sqrt", U"\u221A" },
			{ U"prop", U"\u221D" },
			{ U"propto", U"\u221D" },
			{ U"Proportional", U"\u221D" },
			{ U"vprop", U"\u221D" },
			{ U"varpropto", U"\u221D" },
			{ U"infin", U"\u221E" },
			{ U"angrt", U"\u221F" },
			{ U"ang", U"\u2220" },
			{ U"angle", U"\u2220" },
			{ U"angmsd", U"\u2221" },
			{ U"measuredangle", U"\u2221" },
			{ U"angsph", U"\u2222" },
			{ U"mid", U"\u2223" },
			{ U"VerticalBar", U"\u2223" },
			{ U"smid", U"\u2223" },
			{ U"shortmid", U"\u2223" },
			{ U"nmid", U"\u2224" },
			{ U"NotVerticalBar", U"\u2224" },
			{ U"nsmid", U"\u2224" },
			{ U"nshortmid", U"\u2224" },
			{ U"par", U"\u2225" },
			{ U"parallel", U"\u2225" },
			{ U"DoubleVerticalBar", U"\u2225" },
			{ U"spar", U"\u2225" },
			{ U"shortparallel", U"\u2225" },
			{ U"npar", U"\u2226" },
			{ U"nparallel", U"\u2226" },
			{ U"NotDoubleVerticalBar", U"\u2226" },
			{ U"nspar", U"\u2226" },
			{ U"nshortparallel", U"\u2226" },
			{ U"and", U"\u2227" },
			{ U"wedge", U"\u2227" },
			{ U"or", U"\u2228" },
			{ U"vee", U"\u2228" },
			{ U"cap", U"\u2229" },
			{ U"cup", U"\u222A" },
			{ U"int", U"\u222B" },
			{ U"Integral", U"\u222B" },
			{ U"Int", U"\u222C" },
			{ U"tint", U"\u222D" },
			{ U"iiint", U"\u222D" },
			{ U"conint", U"\u222E" },
			{ U"oint", U"\u222E" },
			{ U"ContourIntegral", U"\u222E" },
			{ U"Conint", U"\u222F" },
			{ U"DoubleContourIntegral", U"\u222F" },
			{ U"Cconint", U"\u2230" },
			{ U"cwint", U"\u2231" },
			{ U"cwconint", U"\u2232" },
			{ U"ClockwiseContourIntegral", U"\u2232" },
			{ U"awconint", U"\u2233" },
			{ U"CounterClockwiseContourIntegral", U"\u2233" },
			{ U"there4", U"\u2234" },
			{ U"therefore", U"\u2234" },
			{ U"Therefore", U"\u2234" },
			{ U"becaus", U"\u2235" },
			{ U"because", U"\u2235" },
			{ U"Because", U"\u2235" },
			{ U"ratio", U"\u2236" },
			{ U"Colon", U"\u2237" },
			{ U"Proportion", U"\u2237" },
			{ U"minusd", U"\u2238" },
			{ U"dotminus", U"\u2238" },
			{ U"mDDot", U"\u223A" },
			{ U"homtht", U"\u223B" },
			{ U"sim", U"\u223C" },
			{ U"Tilde", U"\u223C" },
			{ U"thksim", U"\u223C" },
			{ U"thicksim", U"\u223C" },
			{ U"bsim", U"\u223D" },
			{ U"backsim", U"\u223D" },
			{ U"ac", U"\u223E" },
			{ U"mstpos", U"\u223E" },
			{ U"acd", U"\u223F" },
			{ U"wreath", U"\u2240" },
			{ U"VerticalTilde", U"\u2240" },
			{ U"wr", U"\u2240" },
			{ U"nsim", U"\u2241" },
			{ U"NotTilde", U"\u2241" },
			{ U"esim", U"\u2242" },
			{ U"EqualTilde", U"\u2242" },
			{ U"eqsim", U"\u2242" },
			{ U"sime", U"\u2243" },
			{ U"TildeEqual", U"\u2243" },
			{ U"simeq", U"\u2243" },
			{ U"nsime", U"\u2244" },
			{ U"nsimeq", U"\u2244" },
			{ U"NotTildeEqual", U"\u2244" },
			{ U"cong", U"\u2245" },
			{ U"TildeFullEqual", U"\u2245" },
			{ U"simne", U"\u2246" },
			{ U"ncong", U"\u2247" },
			{ U"NotTildeFullEqual", U"\u2247" },
			{ U"asymp", U"\u2248" },
			{ U"ap", U"\u2248" },
			{ U"TildeTilde", U"\u2248" },
			{ U"approx", U"\u2248" },
			{ U"thkap", U"\u2248" },
			{ U"thickapprox", U"\u2248" },
			{ U"nap", U"\u2249" },
			{ U"NotTildeTilde", U"\u2249" },
			{ U"napprox", U"\u2249" },
			{ U"ape", U"\u224A" },
			{ U"approxeq", U"\u224A" },
			{ U"apid", U"\u224B" },
			{ U"bcong", U"\u224C" },
			{ U"backcong", U"\u224C" },
			{ U"asympeq", U"\u224D" },
			{ U"CupCap", U"\u224D" },
			{ U"bump", U"\u224E" },
			{ U"HumpDownHump", U"\u224E" },
			{ U"Bumpeq", U"\u224E" },
			{ U"bumpe", U"\u224F" },
			{ U"HumpEqual", U"\u224F" },
			{ U"bumpeq", U"\u224F" },
			{ U"esdot", U"\u2250" },
			{ U"DotEqual", U"\u2250" },
			{ U"doteq", U"\u2250" },
			{ U"eDot", U"\u2251" },
			{ U"doteqdot", U"\u2251" },
			{ U"efDot", U"\u2252" },
			{ U"fallingdotseq", U"\u2252" },
			{ U"erDot", U"\u2253" },
			{ U"risingdotseq", U"\u2253" },
			{ U"colone", U"\u2254" },
			{ U"coloneq", U"\u2254" },
			{ U"Assign", U"\u2254" },
			{ U"ecolon", U"\u2255" },
			{ U"eqcolon", U"\u2255" },
			{ U"ecir", U"\u2256" },
			{ U"eqcirc", U"\u2256" },
			{ U"cire", U"\u2257" },
			{ U"circeq", U"\u2257" },
			{ U"wedgeq", U"\u2259" },
			{ U"veeeq", U"\u225A" },
			{ U"trie", U"\u225C" },
			{ U"triangleq", U"\u225C" },
			{ U"equest", U"\u225F" },
			{ U"questeq", U"\u225F" },
			{ U"ne", U"\u2260" },
			{ U"NotEqual", U"\u2260" },
			{ U"equiv", U"\u2261" },
			{ U"Congruent", U"\u2261" },
			{ U"nequiv", U"\u2262" },
			{ U"NotCongruent", U"\u2262" },
			{ U"le", U"\u2264" },
			{ U"leq", U"\u2264" },
			{ U"ge", U"\u2265" },
			{ U"GreaterEqual", U"\u2265" },
			{ U"geq", U"\u2265" },
			{ U"lE", U"\u2266" },
			{ U"LessFullEqual", U"\u2266" },
			{ U"leqq", U"\u2266" },
			{ U"gE", U"\u2267" },
			{ U"GreaterFullEqual", U"\u2267" },
			{ U"geqq", U"\u2267" },
			{ U"lnE", U"\u2268" },
			{ U"lneqq", U"\u2268" },
			{ U"gnE", U"\u2269" },
			{ U"gneqq", U"\u2269" },
			{ U"Lt", U"\u226A" },
			{ U"NestedLessLess", U"\u226A" },
			{ U"ll", U"\u226A" },
			{ U"Gt", U"\u226B" },
			{ U"NestedGreaterGreater", U"\u226B" },
			{ U"gg", U"\u226B" },
			{ U"twixt", U"\u226C" },
			{ U"between", U"\u226C" },
			{ U"NotCupCap", U"\u226D" },
			{ U"nlt", U"\u226E" },
			{ U"NotLess", U"\u226E" },
			{ U"nless", U"\u226E" },
			{ U"ngt", U"\u226F" },
			{ U"NotGreater", U"\u226F" },
			{ U"ngtr", U"\u226F" },
			{ U"nle", U"\u2270" },
			{ U"NotLessEqual", U"\u2270" },
			{ U"nleq", U"\u2270" },
			{ U"nge", U"\u2271" },
			{ U"NotGreaterEqual", U"\u2271" },
			{ U"", U"\u2271" },
			{ U"ngeq", U"\u2272" },
			{ U"LessTilde", U"\u2272" },
			{ U"lesssim", U"\u2272" },
			{ U"gsim", U"\u2273" },
			{ U"gtrsim", U"\u2273" },
			{ U"GreaterTilde", U"\u2273" },
			{ U"nlsim", U"\u2274" },
			{ U"NotLessTilde", U"\u2274" },
			{ U"ngsim", U"\u2275" },
			{ U"NotGreaterTilde", U"\u2275" },
			{ U"lg", U"\u2276" },
			{ U"lessgtr", U"\u2276" },
			{ U"LessGreater", U"\u2276" },
			{ U"gl", U"\u2277" },
			{ U"gtrless", U"\u2277" },
			{ U"GreaterLess", U"\u2277" },
			{ U"ntlg", U"\u2278" },
			{ U"NotLessGreater", U"\u2278" },
			{ U"ntgl", U"\u2279" },
			{ U"NotGreaterLess", U"\u2279" },
			{ U"pr", U"\u227A" },
			{ U"Precedes", U"\u227A" },
			{ U"prec", U"\u227A" },
			{ U"sc", U"\u227B" },
			{ U"Succeeds", U"\u227B" },
			{ U"succ", U"\u227B" },
			{ U"prcue", U"\u227C" },
			{ U"PrecedesSlantEqual", U"\u227C" },
			{ U"preccurlyeq", U"\u227C" },
			{ U"sccue", U"\u227D" },
			{ U"SucceedsSlantEqual", U"\u227D" },
			{ U"succcurlyeq", U"\u227D" },
			{ U"prsim", U"\u227E" },
			{ U"precsim", U"\u227E" },
			{ U"PrecedesTilde", U"\u227E" },
			{ U"scsim", U"\u227F" },
			{ U"succsim", U"\u227F" },
			{ U"SucceedsTilde", U"\u227F" },
			{ U"npr", U"\u2280" },
			{ U"nprec", U"\u2280" },
			{ U"NotPrecedes", U"\u2280" },
			{ U"nsc", U"\u2281" },
			{ U"nsucc", U"\u2281" },
			{ U"NotSucceeds", U"\u2281" },
			{ U"sub", U"\u2282" },
			{ U"subset", U"\u2282" },
			{ U"sup", U"\u2283" },
			{ U"supset", U"\u2283" },
			{ U"Superset", U"\u2283" },
			{ U"nsub", U"\u2284" },
			{ U"nsup", U"\u2285" },
			{ U"sube", U"\u2286" },
			{ U"SubsetEqual", U"\u2286" },
			{ U"subseteq", U"\u2286" },
			{ U"supe", U"\u2287" },
			{ U"supseteq", U"\u2287" },
			{ U"SupersetEqual", U"\u2287" },
			{ U"nsube", U"\u2288" },
			{ U"nsubseteq", U"\u2288" },
			{ U"NotSubsetEqual", U"\u2288" },
			{ U"nsupe", U"\u2289" },
			{ U"nsupseteq", U"\u2289" },
			{ U"NotSupersetEqual", U"\u2289" },
			{ U"subne", U"\u228A" },
			{ U"subsetneq", U"\u228A" },
			{ U"supne", U"\u228B" },
			{ U"supsetneq", U"\u228B" },
			{ U"cupdot", U"\u228D" },
			{ U"uplus", U"\u228E" },
			{ U"UnionPlus", U"\u228E" },
			{ U"sqsub", U"\u228F" },
			{ U"SquareSubset", U"\u228F" },
			{ U"sqsubset", U"\u228F" },
			{ U"sqsup", U"\u2290" },
			{ U"SquareSuperset", U"\u2290" },
			{ U"sqsupset", U"\u2290" },
			{ U"sqsube", U"\u2291" },
			{ U"SquareSubsetEqual", U"\u2291" },
			{ U"sqsubseteq", U"\u2291" },
			{ U"sqsupe", U"\u2292" },
			{ U"SquareSupersetEqual", U"\u2292" },
			{ U"sqsupseteq", U"\u2292" },
			{ U"sqcap", U"\u2293" },
			{ U"SquareIntersection", U"\u2293" },
			{ U"sqcup", U"\u2294" },
			{ U"SquareUnion", U"\u2294" },
			{ U"oplus", U"\u2295" },
			{ U"CirclePlus", U"\u2295" },
			{ U"ominus", U"\u2296" },
			{ U"CircleMinus", U"\u2296" },
			{ U"otimes", U"\u2297" },
			{ U"CircleTimes", U"\u2297" },
			{ U"osol", U"\u2298" },
			{ U"odot", U"\u2299" },
			{ U"CircleDot", U"\u2299" },
			{ U"ocir", U"\u229A" },
			{ U"circledcirc", U"\u229A" },
			{ U"oast", U"\u229B" },
			{ U"circledast", U"\u229B" },
			{ U"odash", U"\u229D" },
			{ U"circleddash", U"\u229D" },
			{ U"plusb", U"\u229E" },
			{ U"boxplus", U"\u229E" },
			{ U"minusb", U"\u229F" },
			{ U"boxminus", U"\u229F" },
			{ U"timesb", U"\u22A0" },
			{ U"boxtimes", U"\u22A0" },
			{ U"sdotb", U"\u22A1" },
			{ U"dotsquare", U"\u22A1" },
			{ U"vdash", U"\u22A2" },
			{ U"RightTee", U"\u22A2" },
			{ U"dashv", U"\u22A3" },
			{ U"LeftTee", U"\u22A3" },
			{ U"top", U"\u22A4" },
			{ U"DownTee", U"\u22A4" },
			{ U"bottom", U"\u22A5" },
			{ U"bot", U"\u22A5" },
			{ U"perp", U"\u22A5" },
			{ U"UpTee", U"\u22A5" },
			{ U"models", U"\u22A7" },
			{ U"vDash", U"\u22A8" },
			{ U"DoubleRightTee", U"\u22A8" },
			{ U"Vdash", U"\u22A9" },
			{ U"Vvdash", U"\u22AA" },
			{ U"VDash", U"\u22AB" },
			{ U"nvdash", U"\u22AC" },
			{ U"nvDash", U"\u22AD" },
			{ U"nVdash", U"\u22AE" },
			{ U"nVDash", U"\u22AF" },
			{ U"prurel", U"\u22B0" },
			{ U"vltri", U"\u22B2" },
			{ U"vartriangleleft", U"\u22B2" },
			{ U"LeftTriangle", U"\u22B2" },
			{ U"vrtri", U"\u22B3" },
			{ U"vartriangleright", U"\u22B3" },
			{ U"RightTriangle", U"\u22B3" },
			{ U"ltrie", U"\u22B4" },
			{ U"trianglelefteq", U"\u22B4" },
			{ U"LeftTriangleEqual", U"\u22B4" },
			{ U"rtrie", U"\u22B5" },
			{ U"trianglerighteq", U"\u22B5" },
			{ U"RightTriangleEqual", U"\u22B5" },
			{ U"origof", U"\u22B6" },
			{ U"imof", U"\u22B7" },
			{ U"mumap", U"\u22B8" },
			{ U"multimap", U"\u22B8" },
			{ U"hercon", U"\u22B9" },
			{ U"intcal", U"\u22BA" },
			{ U"intercal", U"\u22BA" },
			{ U"veebar", U"\u22BB" },
			{ U"barvee", U"\u22BD" },
			{ U"angrtvb", U"\u22BE" },
			{ U"lrtri", U"\u22BF" },
			{ U"xwedge", U"\u22C0" },
			{ U"Wedge", U"\u22C0" },
			{ U"bigwedge", U"\u22C0" },
			{ U"xvee", U"\u22C1" },
			{ U"Vee", U"\u22C1" },
			{ U"bigvee", U"\u22C1" },
			{ U"xcap", U"\u22C2" },
			{ U"Intersection", U"\u22C2" },
			{ U"bigcap", U"\u22C2" },
			{ U"xcup", U"\u22C3" },
			{ U"Union", U"\u22C3" },
			{ U"bigcup", U"\u22C3" },
			{ U"diam", U"\u22C4" },
			{ U"diamond", U"\u22C4" },
			{ U"Diamond", U"\u22C4" },
			{ U"sdot", U"\u22C5" },
			{ U"sstarf", U"\u22C6" },
			{ U"Star", U"\u22C6" },
			{ U"divonx", U"\u22C7" },
			{ U"divideontimes", U"\u22C7" },
			{ U"bowtie", U"\u22C8" },
			{ U"ltimes", U"\u22C9" },
			{ U"rtimes", U"\u22CA" },
			{ U"lthree", U"\u22CB" },
			{ U"leftthreetimes", U"\u22CB" },
			{ U"rthree", U"\u22CC" },
			{ U"rightthreetimes", U"\u22CC" },
			{ U"bsime", U"\u22CD" },
			{ U"backsimeq", U"\u22CD" },
			{ U"cuvee", U"\u22CE" },
			{ U"curlyvee", U"\u22CE" },
			{ U"cuwed", U"\u22CF" },
			{ U"curlywedge", U"\u22CF" },
			{ U"Sub", U"\u22D0" },
			{ U"Subset", U"\u22D0" },
			{ U"Sup", U"\u22D1" },
			{ U"Supset", U"\u22D1" },
			{ U"Cap", U"\u22D2" },
			{ U"Cup", U"\u22D3" },
			{ U"fork", U"\u22D4" },
			{ U"pitchfork", U"\u22D4" },
			{ U"epar", U"\u22D5" },
			{ U"ltdot", U"\u22D6" },
			{ U"lessdot", U"\u22D6" },
			{ U"gtdot", U"\u22D7" },
			{ U"gtrdot", U"\u22D7" },
			{ U"Ll", U"\u22D8" },
			{ U"Gg", U"\u22D9" },
			{ U"ggg", U"\u22D9" },
			{ U"leg", U"\u22DA" },
			{ U"LessEqualGreater", U"\u22DA" },
			{ U"lesseqgtr", U"\u22DA" },
			{ U"gel", U"\u22DB" },
			{ U"gtreqless", U"\u22DB" },
			{ U"GreaterEqualLess", U"\u22DB" },
			{ U"cuepr", U"\u22DE" },
			{ U"curlyeqprec", U"\u22DE" },
			{ U"cuesc", U"\u22DF" },
			{ U"curlyeqsucc", U"\u22DF" },
			{ U"nprcue", U"\u22E0" },
			{ U"NotPrecedesSlantEqual", U"\u22E0" },
			{ U"nsccue", U"\u22E1" },
			{ U"NotSucceedsSlantEqual", U"\u22E1" },
			{ U"nsqsube", U"\u22E2" },
			{ U"NotSquareSubsetEqual", U"\u22E2" },
			{ U"nsqsupe", U"\u22E3" },
			{ U"NotSquareSupersetEqual", U"\u22E3" },
			{ U"lnsim", U"\u22E6" },
			{ U"gnsim", U"\u22E7" },
			{ U"prnsim", U"\u22E8" },
			{ U"precnsim", U"\u22E8" },
			{ U"scnsim", U"\u22E9" },
			{ U"succnsim", U"\u22E9" },
			{ U"nltri", U"\u22EA" },
			{ U"ntriangleleft", U"\u22EA" },
			{ U"NotLeftTriangle", U"\u22EA" },
			{ U"nrtri", U"\u22EB" },
			{ U"ntriangleright", U"\u22EB" },
			{ U"NotRightTriangle", U"\u22EB" },
			{ U"nltrie", U"\u22EC" },
			{ U"ntrianglelefteq", U"\u22EC" },
			{ U"NotLeftTriangleEqual", U"\u22EC" },
			{ U"nrtrie", U"\u22ED" },
			{ U"ntrianglerighteq", U"\u22ED" },
			{ U"NotRightTriangleEqual", U"\u22ED" },
			{ U"vellip", U"\u22EE" },
			{ U"ctdot", U"\u22EF" },
			{ U"utdot", U"\u22F0" },
			{ U"dtdot", U"\u22F1" },
			{ U"disin", U"\u22F2" },
			{ U"isinsv", U"\u22F3" },
			{ U"isins", U"\u22F4" },
			{ U"isindot", U"\u22F5" },
			{ U"notinvc", U"\u22F6" },
			{ U"notinvb", U"\u22F7" },
			{ U"isinE", U"\u22F9" },
			{ U"nisd", U"\u22FA" },
			{ U"xnis", U"\u22FB" },
			{ U"nis", U"\u22FC" },
			{ U"notnivc", U"\u22FD" },
			{ U"notnivb", U"\u22FE" },
			{ U"barwed", U"\u2305" },
			{ U"barwedge", U"\u2305" },
			{ U"Barwed", U"\u2306" },
			{ U"doublebarwedge", U"\u2306" },
			{ U"lceil", U"\u2308" },
			{ U"LeftCeiling", U"\u2308" },
			{ U"rceil", U"\u2309" },
			{ U"RightCeiling", U"\u2309" },
			{ U"lfloor", U"\u230A" },
			{ U"LeftFloor", U"\u230A" },
			{ U"rfloor", U"\u230B" },
			{ U"RightFloor", U"\u230B" },
			{ U"drcrop", U"\u230C" },
			{ U"dlcrop", U"\u230D" },
			{ U"urcrop", U"\u230E" },
			{ U"ulcrop", U"\u230F" },
			{ U"bnot", U"\u2310" },
			{ U"profline", U"\u2312" },
			{ U"profsurf", U"\u2313" },
			{ U"telrec", U"\u2315" },
			{ U"target", U"\u2316" },
			{ U"ulcorn", U"\u231C" },
			{ U"ulcorner", U"\u231C" },
			{ U"urcorn", U"\u231D" },
			{ U"urcorner", U"\u231D" },
			{ U"dlcorn", U"\u231E" },
			{ U"llcorner", U"\u231E" },
			{ U"drcorn", U"\u231F" },
			{ U"lrcorner", U"\u231F" },
			{ U"frown", U"\u2322" },
			{ U"sfrown", U"\u2322" },
			{ U"smile", U"\u2323" },
			{ U"ssmile", U"\u2323" },
			{ U"cylcty", U"\u232D" },
			{ U"profalar", U"\u232E" },
			{ U"topbot", U"\u2336" },
			{ U"ovbar", U"\u233D" },
			{ U"solbar", U"\u233F" },
			{ U"angzarr", U"\u237C" },
			{ U"lmoust", U"\u23B0" },
			{ U"lmoustache", U"\u23B0" },
			{ U"rmoust", U"\u23B1" },
			{ U"rmoustache", U"\u23B1" },
			{ U"tbrk", U"\u23B4" },
			{ U"OverBracket", U"\u23B4" },
			{ U"bbrk", U"\u23B5" },
			{ U"UnderBracket", U"\u23B5" },
			{ U"bbrktbrk", U"\u23B6" },
			{ U"OverParenthesis", U"\u23DC" },
			{ U"UnderParenthesis", U"\u23DD" },
			{ U"OverBrace", U"\u23DE" },
			{ U"UnderBrace", U"\u23DF" },
			{ U"trpezium", U"\u23E2" },
			{ U"elinters", U"\u23E7" },
			{ U"blank", U"\u2423" },
			{ U"oS", U"\u24C8" },
			{ U"circledS", U"\u24C8" },
			{ U"boxh", U"\u2500" },
			{ U"HorizontalLine", U"\u2500" },
			{ U"boxv", U"\u2502" },
			{ U"boxdr", U"\u250C" },
			{ U"boxdl", U"\u2510" },
			{ U"boxur", U"\u2514" },
			{ U"boxul", U"\u2518" },
			{ U"boxvr", U"\u251C" },
			{ U"boxvl", U"\u2524" },
			{ U"boxhd", U"\u252C" },
			{ U"boxhu", U"\u2534" },
			{ U"boxvh", U"\u253C" },
			{ U"boxH", U"\u2550" },
			{ U"boxV", U"\u2551" },
			{ U"boxdR", U"\u2552" },
			{ U"boxDr", U"\u2553" },
			{ U"boxDR", U"\u2554" },
			{ U"boxdL", U"\u2555" },
			{ U"boxDl", U"\u2556" },
			{ U"boxDL", U"\u2557" },
			{ U"boxuR", U"\u2558" },
			{ U"boxUr", U"\u2559" },
			{ U"boxUR", U"\u255A" },
			{ U"boxuL", U"\u255B" },
			{ U"boxUl", U"\u255C" },
			{ U"boxUL", U"\u255D" },
			{ U"boxvR", U"\u255E" },
			{ U"boxVr", U"\u255F" },
			{ U"boxVR", U"\u2560" },
			{ U"boxvL", U"\u2561" },
			{ U"boxVl", U"\u2562" },
			{ U"boxVL", U"\u2563" },
			{ U"boxHd", U"\u2564" },
			{ U"boxhD", U"\u2565" },
			{ U"boxHD", U"\u2566" },
			{ U"boxHu", U"\u2567" },
			{ U"boxhU", U"\u2568" },
			{ U"boxHU", U"\u2569" },
			{ U"boxvH", U"\u256A" },
			{ U"boxVh", U"\u256B" },
			{ U"boxVH", U"\u256C" },
			{ U"uhblk", U"\u2580" },
			{ U"lhblk", U"\u2584" },
			{ U"block", U"\u2588" },
			{ U"blk14", U"\u2591" },
			{ U"blk12", U"\u2592" },
			{ U"blk34", U"\u2593" },
			{ U"squ", U"\u25A1" },
			{ U"square", U"\u25A1" },
			{ U"Square", U"\u25A1" },
			{ U"squf", U"\u25AA" },
			{ U"squarf", U"\u25AA" },
			{ U"blacksquare", U"\u25AA" },
			{ U"FilledVerySmallSquare", U"\u25AA" },
			{ U"EmptyVerySmallSquare", U"\u25AB" },
			{ U"rect", U"\u25AD" },
			{ U"marker", U"\u25AE" },
			{ U"fltns", U"\u25B1" },
			{ U"xutri", U"\u25B3" },
			{ U"bigtriangleup", U"\u25B3" },
			{ U"utrif", U"\u25B4" },
			{ U"blacktriangle", U"\u25B4" },
			{ U"utri", U"\u25B5" },
			{ U"triangle", U"\u25B5" },
			{ U"rtrif", U"\u25B8" },
			{ U"blacktriangleright", U"\u25B8" },
			{ U"rtri", U"\u25B9" },
			{ U"triangleright", U"\u25B9" },
			{ U"xdtri", U"\u25BD" },
			{ U"bigtriangledown", U"\u25BD" },
			{ U"dtrif", U"\u25BE" },
			{ U"blacktriangledown", U"\u25BE" },
			{ U"dtri", U"\u25BF" },
			{ U"triangledown", U"\u25BF" },
			{ U"ltrif", U"\u25C2" },
			{ U"blacktriangleleft", U"\u25C2" },
			{ U"ltri", U"\u25C3" },
			{ U"triangleleft", U"\u25C3" },
			{ U"loz", U"\u25CA" },
			{ U"lozenge", U"\u25CA" },
			{ U"cir", U"\u25CB" },
			{ U"tridot", U"\u25EC" },
			{ U"xcirc", U"\u25EF" },
			{ U"bigcirc", U"\u25EF" },
			{ U"ultri", U"\u25F8" },
			{ U"urtri", U"\u25F9" },
			{ U"lltri", U"\u25FA" },
			{ U"EmptySmallSquare", U"\u25FB" },
			{ U"FilledSmallSquare", U"\u25FC" },
			{ U"starf", U"\u2605" },
			{ U"bigstar", U"\u2605" },
			{ U"star", U"\u2606" },
			{ U"phone", U"\u260E" },
			{ U"female", U"\u2640" },
			{ U"male", U"\u2642" },
			{ U"spades", U"\u2660" },
			{ U"spadesuit", U"\u2660" },
			{ U"clubs", U"\u2663" },
			{ U"clubsuit", U"\u2663" },
			{ U"hearts", U"\u2665" },
			{ U"heartsuit", U"\u2665" },
			{ U"diams", U"\u2666" },
			{ U"diamondsuit", U"\u2666" },
			{ U"sung", U"\u266A" },
			{ U"flat", U"\u266D" },
			{ U"natur", U"\u266E" },
			{ U"natural", U"\u266E" },
			{ U"sharp", U"\u266F" },
			{ U"check", U"\u2713" },
			{ U"checkmark", U"\u2713" },
			{ U"cross", U"\u2717" },
			{ U"malt", U"\u2720" },
			{ U"maltese", U"\u2720" },
			{ U"sext", U"\u2736" },
			{ U"VerticalSeparator", U"\u2758" },
			{ U"lbbrk", U"\u2772" },
			{ U"rbbrk", U"\u2773" },
			{ U"bsolhsub", U"\u27C8" },
			{ U"suphsol", U"\u27C9" },
			{ U"lobrk", U"\u27E6" },
			{ U"LeftDoubleBracket", U"\u27E6" },
			{ U"robrk", U"\u27E7" },
			{ U"RightDoubleBracket", U"\u27E7" },
			{ U"lang", U"\u27E8" },
			{ U"LeftAngleBracket", U"\u27E8" },
			{ U"langle", U"\u27E8" },
			{ U"rang", U"\u27E9" },
			{ U"RightAngleBracket", U"\u27E9" },
			{ U"rangle", U"\u27E9" },
			{ U"Lang", U"\u27EA" },
			{ U"Rang", U"\u27EB" },
			{ U"loang", U"\u27EC" },
			{ U"roang", U"\u27ED" },
			{ U"xlarr", U"\u27F5" },
			{ U"longleftarrow", U"\u27F5" },
			{ U"LongLeftArrow", U"\u27F5" },
			{ U"xrarr", U"\u27F6" },
			{ U"longrightarrow", U"\u27F6" },
			{ U"LongRightArrow", U"\u27F6" },
			{ U"xharr", U"\u27F7" },
			{ U"longleftrightarrow", U"\u27F7" },
			{ U"LongLeftRightArrow", U"\u27F7" },
			{ U"xlArr", U"\u27F8" },
			{ U"Longleftarrow", U"\u27F8" },
			{ U"DoubleLongLeftArrow", U"\u27F8" },
			{ U"xrArr", U"\u27F9" },
			{ U"Longrightarrow", U"\u27F9" },
			{ U"DoubleLongRightArrow", U"\u27F9" },
			{ U"xhArr", U"\u27FA" },
			{ U"Longleftrightarrow", U"\u27FA" },
			{ U"DoubleLongLeftRightArrow", U"\u27FA" },
			{ U"xmap", U"\u27FC" },
			{ U"longmapsto", U"\u27FC" },
			{ U"dzigrarr", U"\u27FF" },
			{ U"nvlArr", U"\u2902" },
			{ U"nvrArr", U"\u2903" },
			{ U"nvHarr", U"\u2904" },
			{ U"Map", U"\u2905" },
			{ U"lbarr", U"\u290C" },
			{ U"rbarr", U"\u290D" },
			{ U"bkarow", U"\u290D" },
			{ U"lBarr", U"\u290E" },
			{ U"rBarr", U"\u290F" },
			{ U"dbkarow", U"\u290F" },
			{ U"RBarr", U"\u2910" },
			{ U"drbkarow", U"\u2910" },
			{ U"DDotrahd", U"\u2911" },
			{ U"UpArrowBar", U"\u2912" },
			{ U"DownArrowBar", U"\u2913" },
			{ U"Rarrtl", U"\u2916" },
			{ U"latail", U"\u2919" },
			{ U"ratail", U"\u291A" },
			{ U"lAtail", U"\u291B" },
			{ U"rAtail", U"\u291C" },
			{ U"larrfs", U"\u291D" },
			{ U"rarrfs", U"\u291E" },
			{ U"larrbfs", U"\u291F" },
			{ U"rarrbfs", U"\u2920" },
			{ U"nwarhk", U"\u2923" },
			{ U"nearhk", U"\u2924" },
			{ U"searhk", U"\u2925" },
			{ U"hksearow", U"\u2925" },
			{ U"swarhk", U"\u2926" },
			{ U"hkswarow", U"\u2926" },
			{ U"nwnear", U"\u2927" },
			{ U"nesear", U"\u2928" },
			{ U"toea", U"\u2928" },
			{ U"seswar", U"\u2929" },
			{ U"tosa", U"\u2929" },
			{ U"swnwar", U"\u292A" },
			{ U"rarrc", U"\u2933" },
			{ U"cudarrr", U"\u2935" },
			{ U"ldca", U"\u2936" },
			{ U"rdca", U"\u2937" },
			{ U"cudarrl", U"\u2938" },
			{ U"larrpl", U"\u2939" },
			{ U"curarrm", U"\u293C" },
			{ U"cularrp", U"\u293D" },
			{ U"rarrpl", U"\u2945" },
			{ U"harrcir", U"\u2948" },
			{ U"Uarrocir", U"\u2949" },
			{ U"lurdshar", U"\u294A" },
			{ U"ldrushar", U"\u294B" },
			{ U"LeftRightVector", U"\u294E" },
			{ U"RightUpDownVector", U"\u294F" },
			{ U"DownLeftRightVector", U"\u2950" },
			{ U"LeftUpDownVector", U"\u2951" },
			{ U"LeftVectorBar", U"\u2952" },
			{ U"RightVectorBar", U"\u2953" },
			{ U"RightUpVectorBar", U"\u2954" },
			{ U"RightDownVectorBar", U"\u2955" },
			{ U"DownLeftVectorBar", U"\u2956" },
			{ U"DownRightVectorBar", U"\u2957" },
			{ U"LeftUpVectorBar", U"\u2958" },
			{ U"LeftDownVectorBar", U"\u2959" },
			{ U"LeftTeeVector", U"\u295A" },
			{ U"RightTeeVector", U"\u295B" },
			{ U"RightUpTeeVector", U"\u295C" },
			{ U"RightDownTeeVector", U"\u295D" },
			{ U"DownLeftTeeVector", U"\u295E" },
			{ U"DownRightTeeVector", U"\u295F" },
			{ U"LeftUpTeeVector", U"\u2960" },
			{ U"LeftDownTeeVector", U"\u2961" },
			{ U"lHar", U"\u2962" },
			{ U"uHar", U"\u2963" },
			{ U"rHar", U"\u2964" },
			{ U"dHar", U"\u2965" },
			{ U"luruhar", U"\u2966" },
			{ U"ldrdhar", U"\u2967" },
			{ U"ruluhar", U"\u2968" },
			{ U"rdldhar", U"\u2969" },
			{ U"lharul", U"\u296A" },
			{ U"llhard", U"\u296B" },
			{ U"rharul", U"\u296C" },
			{ U"lrhard", U"\u296D" },
			{ U"udhar", U"\u296E" },
			{ U"UpEquilibrium", U"\u296E" },
			{ U"duhar", U"\u296F" },
			{ U"ReverseUpEquilibrium", U"\u296F" },
			{ U"RoundImplies", U"\u2970" },
			{ U"erarr", U"\u2971" },
			{ U"simrarr", U"\u2972" },
			{ U"larrsim", U"\u2973" },
			{ U"rarrsim", U"\u2974" },
			{ U"rarrap", U"\u2975" },
			{ U"ltlarr", U"\u2976" },
			{ U"gtrarr", U"\u2978" },
			{ U"subrarr", U"\u2979" },
			{ U"suplarr", U"\u297B" },
			{ U"lfisht", U"\u297C" },
			{ U"rfisht", U"\u297D" },
			{ U"ufisht", U"\u297E" },
			{ U"dfisht", U"\u297F" },
			{ U"lopar", U"\u2985" },
			{ U"ropar", U"\u2986" },
			{ U"lbrke", U"\u298B" },
			{ U"rbrke", U"\u298C" },
			{ U"lbrkslu", U"\u298D" },
			{ U"rbrksld", U"\u298E" },
			{ U"lbrksld", U"\u298F" },
			{ U"rbrkslu", U"\u2990" },
			{ U"langd", U"\u2991" },
			{ U"rangd", U"\u2992" },
			{ U"lparlt", U"\u2993" },
			{ U"rpargt", U"\u2994" },
			{ U"gtlPar", U"\u2995" },
			{ U"ltrPar", U"\u2996" },
			{ U"vzigzag", U"\u299A" },
			{ U"vangrt", U"\u299C" },
			{ U"angrtvbd", U"\u299D" },
			{ U"ange", U"\u29A4" },
			{ U"range", U"\u29A5" },
			{ U"dwangle", U"\u29A6" },
			{ U"uwangle", U"\u29A7" },
			{ U"angmsdaa", U"\u29A8" },
			{ U"angmsdab", U"\u29A9" },
			{ U"angmsdac", U"\u29AA" },
			{ U"angmsdad", U"\u29AB" },
			{ U"angmsdae", U"\u29AC" },
			{ U"angmsdaf", U"\u29AD" },
			{ U"angmsdag", U"\u29AE" },
			{ U"angmsdah", U"\u29AF" },
			{ U"bemptyv", U"\u29B0" },
			{ U"demptyv", U"\u29B1" },
			{ U"cemptyv", U"\u29B2" },
			{ U"raemptyv", U"\u29B3" },
			{ U"laemptyv", U"\u29B4" },
			{ U"ohbar", U"\u29B5" },
			{ U"omid", U"\u29B6" },
			{ U"opar", U"\u29B7" },
			{ U"operp", U"\u29B9" },
			{ U"olcross", U"\u29BB" },
			{ U"odsold", U"\u29BC" },
			{ U"olcir", U"\u29BE" },
			{ U"ofcir", U"\u29BF" },
			{ U"olt", U"\u29C0" },
			{ U"ogt", U"\u29C1" },
			{ U"cirscir", U"\u29C2" },
			{ U"cirE", U"\u29C3" },
			{ U"solb", U"\u29C4" },
			{ U"bsolb", U"\u29C5" },
			{ U"boxbox", U"\u29C9" },
			{ U"trisb", U"\u29CD" },
			{ U"rtriltri", U"\u29CE" },
			{ U"LeftTriangleBar", U"\u29CF" },
			{ U"RightTriangleBar", U"\u29D0" },
			{ U"iinfin", U"\u29DC" },
			{ U"infintie", U"\u29DD" },
			{ U"nvinfin", U"\u29DE" },
			{ U"eparsl", U"\u29E3" },
			{ U"smeparsl", U"\u29E4" },
			{ U"eqvparsl", U"\u29E5" },
			{ U"lozf", U"\u29EB" },
			{ U"blacklozenge", U"\u29EB" },
			{ U"RuleDelayed", U"\u29F4" },
			{ U"dsol", U"\u29F6" },
			{ U"xodot", U"\u2A00" },
			{ U"bigodot", U"\u2A00" },
			{ U"xoplus", U"\u2A01" },
			{ U"bigoplus", U"\u2A01" },
			{ U"xotime", U"\u2A02" },
			{ U"bigotimes", U"\u2A02" },
			{ U"xuplus", U"\u2A04" },
			{ U"biguplus", U"\u2A04" },
			{ U"xsqcup", U"\u2A06" },
			{ U"bigsqcup", U"\u2A06" },
			{ U"qint", U"\u2A0C" },
			{ U"iiiint", U"\u2A0C" },
			{ U"fpartint", U"\u2A0D" },
			{ U"cirfnint", U"\u2A10" },
			{ U"awint", U"\u2A11" },
			{ U"rppolint", U"\u2A12" },
			{ U"scpolint", U"\u2A13" },
			{ U"npolint", U"\u2A14" },
			{ U"pointint", U"\u2A15" },
			{ U"quatint", U"\u2A16" },
			{ U"intlarhk", U"\u2A17" },
			{ U"pluscir", U"\u2A22" },
			{ U"plusacir", U"\u2A23" },
			{ U"simplus", U"\u2A24" },
			{ U"plusdu", U"\u2A25" },
			{ U"plussim", U"\u2A26" },
			{ U"plustwo", U"\u2A27" },
			{ U"mcomma", U"\u2A29" },
			{ U"minusdu", U"\u2A2A" },
			{ U"loplus", U"\u2A2D" },
			{ U"roplus", U"\u2A2E" },
			{ U"Cross", U"\u2A2F" },
			{ U"timesd", U"\u2A30" },
			{ U"timesbar", U"\u2A31" },
			{ U"smashp", U"\u2A33" },
			{ U"lotimes", U"\u2A34" },
			{ U"rotimes", U"\u2A35" },
			{ U"otimesas", U"\u2A36" },
			{ U"Otimes", U"\u2A37" },
			{ U"odiv", U"\u2A38" },
			{ U"triplus", U"\u2A39" },
			{ U"triminus", U"\u2A3A" },
			{ U"tritime", U"\u2A3B" },
			{ U"iprod", U"\u2A3C" },
			{ U"intprod", U"\u2A3C" },
			{ U"amalg", U"\u2A3F" },
			{ U"capdot", U"\u2A40" },
			{ U"ncup", U"\u2A42" },
			{ U"ncap", U"\u2A43" },
			{ U"capand", U"\u2A44" },
			{ U"cupor", U"\u2A45" },
			{ U"cupcap", U"\u2A46" },
			{ U"capcup", U"\u2A47" },
			{ U"cupbrcap", U"\u2A48" },
			{ U"capbrcup", U"\u2A49" },
			{ U"cupcup", U"\u2A4A" },
			{ U"capcap", U"\u2A4B" },
			{ U"ccups", U"\u2A4C" },
			{ U"ccaps", U"\u2A4D" },
			{ U"ccupssm", U"\u2A50" },
			{ U"And", U"\u2A53" },
			{ U"Or", U"\u2A54" },
			{ U"andand", U"\u2A55" },
			{ U"oror", U"\u2A56" },
			{ U"orslope", U"\u2A57" },
			{ U"andslope", U"\u2A58" },
			{ U"andv", U"\u2A5A" },
			{ U"orv", U"\u2A5B" },
			{ U"andd", U"\u2A5C" },
			{ U"ord", U"\u2A5D" },
			{ U"wedbar", U"\u2A5F" },
			{ U"sdote", U"\u2A66" },
			{ U"simdot", U"\u2A6A" },
			{ U"congdot", U"\u2A6D" },
			{ U"easter", U"\u2A6E" },
			{ U"apacir", U"\u2A6F" },
			{ U"apE", U"\u2A70" },
			{ U"eplus", U"\u2A71" },
			{ U"pluse", U"\u2A72" },
			{ U"Esim", U"\u2A73" },
			{ U"Colone", U"\u2A74" },
			{ U"Equal", U"\u2A75" },
			{ U"eDDot", U"\u2A77" },
			{ U"ddotseq", U"\u2A77" },
			{ U"equivDD", U"\u2A78" },
			{ U"ltcir", U"\u2A79" },
			{ U"gtcir", U"\u2A7A" },
			{ U"ltquest", U"\u2A7B" },
			{ U"gtquest", U"\u2A7C" },
			{ U"les", U"\u2A7D" },
			{ U"LessSlantEqual", U"\u2A7D" },
			{ U"leqslant", U"\u2A7D" },
			{ U"ges", U"\u2A7E" },
			{ U"GreaterSlantEqual", U"\u2A7E" },
			{ U"geqslant", U"\u2A7E" },
			{ U"lesdot", U"\u2A7F" },
			{ U"gesdot", U"\u2A80" },
			{ U"lesdoto", U"\u2A81" },
			{ U"gesdoto", U"\u2A82" },
			{ U"lesdotor", U"\u2A83" },
			{ U"gesdotol", U"\u2A84" },
			{ U"lap", U"\u2A85" },
			{ U"lessapprox", U"\u2A85" },
			{ U"gap", U"\u2A86" },
			{ U"gtrapprox", U"\u2A86" },
			{ U"lne", U"\u2A87" },
			{ U"lneq", U"\u2A87" },
			{ U"gne", U"\u2A88" },
			{ U"gneq", U"\u2A88" },
			{ U"lnap", U"\u2A89" },
			{ U"lnapprox", U"\u2A89" },
			{ U"gnap", U"\u2A8A" },
			{ U"gnapprox", U"\u2A8A" },
			{ U"lEg", U"\u2A8B" },
			{ U"lesseqqgtr", U"\u2A8B" },
			{ U"gEl", U"\u2A8C" },
			{ U"gtreqqless", U"\u2A8C" },
			{ U"lsime", U"\u2A8D" },
			{ U"gsime", U"\u2A8E" },
			{ U"lsimg", U"\u2A8F" },
			{ U"gsiml", U"\u2A90" },
			{ U"lgE", U"\u2A91" },
			{ U"glE", U"\u2A92" },
			{ U"lesges", U"\u2A93" },
			{ U"gesles", U"\u2A94" },
			{ U"els", U"\u2A95" },
			{ U"eqslantless", U"\u2A95" },
			{ U"egs", U"\u2A96" },
			{ U"eqslantgtr", U"\u2A96" },
			{ U"elsdot", U"\u2A97" },
			{ U"egsdot", U"\u2A98" },
			{ U"el", U"\u2A99" },
			{ U"eg", U"\u2A9A" },
			{ U"siml", U"\u2A9D" },
			{ U"simg", U"\u2A9E" },
			{ U"simlE", U"\u2A9F" },
			{ U"simgE", U"\u2AA0" },
			{ U"LessLess", U"\u2AA1" },
			{ U"GreaterGreater", U"\u2AA2" },
			{ U"glj", U"\u2AA4" },
			{ U"gla", U"\u2AA5" },
			{ U"ltcc", U"\u2AA6" },
			{ U"gtcc", U"\u2AA7" },
			{ U"lescc", U"\u2AA8" },
			{ U"gescc", U"\u2AA9" },
			{ U"smt", U"\u2AAA" },
			{ U"lat", U"\u2AAB" },
			{ U"smte", U"\u2AAC" },
			{ U"late", U"\u2AAD" },
			{ U"bumpE", U"\u2AAE" },
			{ U"pre", U"\u2AAF" },
			{ U"preceq", U"\u2AAF" },
			{ U"PrecedesEqual", U"\u2AAF" },
			{ U"sce", U"\u2AB0" },
			{ U"succeq", U"\u2AB0" },
			{ U"SucceedsEqual", U"\u2AB0" },
			{ U"prE", U"\u2AB3" },
			{ U"scE", U"\u2AB4" },
			{ U"prnE", U"\u2AB5" },
			{ U"precneqq", U"\u2AB5" },
			{ U"scnE", U"\u2AB6" },
			{ U"succneqq", U"\u2AB6" },
			{ U"prap", U"\u2AB7" },
			{ U"precapprox", U"\u2AB7" },
			{ U"scap", U"\u2AB8" },
			{ U"succapprox", U"\u2AB8" },
			{ U"prnap", U"\u2AB9" },
			{ U"precnapprox", U"\u2AB9" },
			{ U"scnap", U"\u2ABA" },
			{ U"succnapprox", U"\u2ABA" },
			{ U"Pr", U"\u2ABB" },
			{ U"Sc", U"\u2ABC" },
			{ U"subdot", U"\u2ABD" },
			{ U"supdot", U"\u2ABE" },
			{ U"subplus", U"\u2ABF" },
			{ U"supplus", U"\u2AC0" },
			{ U"submult", U"\u2AC1" },
			{ U"supmult", U"\u2AC2" },
			{ U"subedot", U"\u2AC3" },
			{ U"supedot", U"\u2AC4" },
			{ U"subE", U"\u2AC5" },
			{ U"subseteqq", U"\u2AC5" },
			{ U"supE", U"\u2AC6" },
			{ U"supseteqq", U"\u2AC6" },
			{ U"subsim", U"\u2AC7" },
			{ U"supsim", U"\u2AC8" },
			{ U"subnE", U"\u2ACB" },
			{ U"subsetneqq", U"\u2ACB" },
			{ U"supnE", U"\u2ACC" },
			{ U"supsetneqq", U"\u2ACC" },
			{ U"csub", U"\u2ACF" },
			{ U"csup", U"\u2AD0" },
			{ U"csube", U"\u2AD1" },
			{ U"csupe", U"\u2AD2" },
			{ U"subsup", U"\u2AD3" },
			{ U"supsub", U"\u2AD4" },
			{ U"subsub", U"\u2AD5" },
			{ U"supsup", U"\u2AD6" },
			{ U"suphsub", U"\u2AD7" },
			{ U"supdsub", U"\u2AD8" },
			{ U"forkv", U"\u2AD9" },
			{ U"topfork", U"\u2ADA" },
			{ U"mlcp", U"\u2ADB" },
			{ U"Dashv", U"\u2AE4" },
			{ U"DoubleLeftTee", U"\u2AE4" },
			{ U"Vdashl", U"\u2AE6" },
			{ U"Barv", U"\u2AE7" },
			{ U"vBar", U"\u2AE8" },
			{ U"vBarv", U"\u2AE9" },
			{ U"Vbar", U"\u2AEB" },
			{ U"Not", U"\u2AEC" },
			{ U"bNot", U"\u2AED" },
			{ U"rnmid", U"\u2AEE" },
			{ U"cirmid", U"\u2AEF" },
			{ U"midcir", U"\u2AF0" },
			{ U"topcir", U"\u2AF1" },
			{ U"nhpar", U"\u2AF2" },
			{ U"parsim", U"\u2AF3" },
			{ U"parsl", U"\u2AFD" },
			{ U"fflig", U"\uFB00" },
			{ U"filig", U"\uFB01" },
			{ U"fllig", U"\uFB02" },
			{ U"ffilig", U"\uFB03" },
			{ U"ffllig", U"\uFB04" },
			{ U"Ascr", U"\U0001D49C" },
			{ U"Cscr", U"\U0001D49E" },
			{ U"Dscr", U"\U0001D49F" },
			{ U"Gscr", U"\U0001D4A2" },
			{ U"Jscr", U"\U0001D4A5" },
			{ U"Kscr", U"\U0001D4A6" },
			{ U"Nscr", U"\U0001D4A9" },
			{ U"Oscr", U"\U0001D4AA" },
			{ U"Pscr", U"\U0001D4AB" },
			{ U"Qscr", U"\U0001D4AC" },
			{ U"Sscr", U"\U0001D4AE" },
			{ U"Tscr", U"\U0001D4AF" },
			{ U"Uscr", U"\U0001D4B0" },
			{ U"Vscr", U"\U0001D4B1" },
			{ U"Wscr", U"\U0001D4B2" },
			{ U"Xscr", U"\U0001D4B3" },
			{ U"Yscr", U"\U0001D4B4" },
			{ U"Zscr", U"\U0001D4B5" },
			{ U"ascr", U"\U0001D4B6" },
			{ U"bscr", U"\U0001D4B7" },
			{ U"cscr", U"\U0001D4B8" },
			{ U"dscr", U"\U0001D4B9" },
			{ U"fscr", U"\U0001D4BB" },
			{ U"hscr", U"\U0001D4BD" },
			{ U"iscr", U"\U0001D4BE" },
			{ U"jscr", U"\U0001D4BF" },
			{ U"kscr", U"\U0001D4C0" },
			{ U"lscr", U"\U0001D4C1" },
			{ U"mscr", U"\U0001D4C2" },
			{ U"nscr", U"\U0001D4C3" },
			{ U"pscr", U"\U0001D4C5" },
			{ U"qscr", U"\U0001D4C6" },
			{ U"rscr", U"\U0001D4C7" },
			{ U"sscr", U"\U0001D4C8" },
			{ U"tscr", U"\U0001D4C9" },
			{ U"uscr", U"\U0001D4CA" },
			{ U"vscr", U"\U0001D4CB" },
			{ U"wscr", U"\U0001D4CC" },
			{ U"xscr", U"\U0001D4CD" },
			{ U"yscr", U"\U0001D4CE" },
			{ U"zscr", U"\U0001D4CF" },
			{ U"Afr", U"\U0001D504" },
			{ U"Bfr", U"\U0001D505" },
			{ U"Dfr", U"\U0001D507" },
			{ U"Efr", U"\U0001D508" },
			{ U"Ffr", U"\U0001D509" },
			{ U"Gfr", U"\U0001D50A" },
			{ U"Jfr", U"\U0001D50D" },
			{ U"Kfr", U"\U0001D50E" },
			{ U"Lfr", U"\U0001D50F" },
			{ U"Mfr", U"\U0001D510" },
			{ U"Nfr", U"\U0001D511" },
			{ U"Ofr", U"\U0001D512" },
			{ U"Pfr", U"\U0001D513" },
			{ U"Qfr", U"\U0001D514" },
			{ U"Sfr", U"\U0001D516" },
			{ U"Tfr", U"\U0001D517" },
			{ U"Ufr", U"\U0001D518" },
			{ U"Vfr", U"\U0001D519" },
			{ U"Wfr", U"\U0001D51A" },
			{ U"Xfr", U"\U0001D51B" },
			{ U"Yfr", U"\U0001D51C" },
			{ U"afr", U"\U0001D51E" },
			{ U"bfr", U"\U0001D51F" },
			{ U"cfr", U"\U0001D520" },
			{ U"dfr", U"\U0001D521" },
			{ U"efr", U"\U0001D522" },
			{ U"ffr", U"\U0001D523" },
			{ U"gfr", U"\U0001D524" },
			{ U"hfr", U"\U0001D525" },
			{ U"ifr", U"\U0001D526" },
			{ U"jfr", U"\U0001D527" },
			{ U"kfr", U"\U0001D528" },
			{ U"lfr", U"\U0001D529" },
			{ U"mfr", U"\U0001D52A" },
			{ U"nfr", U"\U0001D52B" },
			{ U"ofr", U"\U0001D52C" },
			{ U"pfr", U"\U0001D52D" },
			{ U"qfr", U"\U0001D52E" },
			{ U"rfr", U"\U0001D52F" },
			{ U"sfr", U"\U0001D530" },
			{ U"tfr", U"\U0001D531" },
			{ U"ufr", U"\U0001D532" },
			{ U"vfr", U"\U0001D533" },
			{ U"wfr", U"\U0001D534" },
			{ U"xfr", U"\U0001D535" },
			{ U"yfr", U"\U0001D536" },
			{ U"zfr", U"\U0001D537" },
			{ U"Aopf", U"\U0001D538" },
			{ U"Bopf", U"\U0001D539" },
			{ U"Dopf", U"\U0001D53B" },
			{ U"Eopf", U"\U0001D53C" },
			{ U"Fopf", U"\U0001D53D" },
			{ U"Gopf", U"\U0001D53E" },
			{ U"Iopf", U"\U0001D540" },
			{ U"Jopf", U"\U0001D541" },
			{ U"Kopf", U"\U0001D542" },
			{ U"Lopf", U"\U0001D543" },
			{ U"Mopf", U"\U0001D544" },
			{ U"Oopf", U"\U0001D546" },
			{ U"Sopf", U"\U0001D54A" },
			{ U"Topf", U"\U0001D54B" },
			{ U"Uopf", U"\U0001D54C" },
			{ U"Vopf", U"\U0001D54D" },
			{ U"Wopf", U"\U0001D54E" },
			{ U"Xopf", U"\U0001D54F" },
			{ U"Yopf", U"\U0001D550" },
			{ U"aopf", U"\U0001D552" },
			{ U"bopf", U"\U0001D553" },
			{ U"copf", U"\U0001D554" },
			{ U"dopf", U"\U0001D555" },
			{ U"eopf", U"\U0001D556" },
			{ U"fopf", U"\U0001D557" },
			{ U"gopf", U"\U0001D558" },
			{ U"hopf", U"\U0001D559" },
			{ U"iopf", U"\U0001D55A" },
			{ U"jopf", U"\U0001D55B" },
			{ U"kopf", U"\U0001D55C" },
			{ U"lopf", U"\U0001D55D" },
			{ U"mopf", U"\U0001D55E" },
			{ U"nopf", U"\U0001D55F" },
			{ U"oopf", U"\U0001D560" },
			{ U"popf", U"\U0001D561" },
			{ U"qopf", U"\U0001D562" },
			{ U"ropf", U"\U0001D563" },
			{ U"sopf", U"\U0001D564" },
			{ U"topf", U"\U0001D565" },
			{ U"uopf", U"\U0001D566" },
			{ U"vopf", U"\U0001D567" },
			{ U"wopf", U"\U0001D568" },
			{ U"xopf", U"\U0001D569" },
			{ U"yopf", U"\U0001D56A" },
			{ U"zopf", U"\U0001D56B" },
			{ U"nvlt", U"\u003C\u20D2" },
			{ U"bne", U"\u003D\u20E5" },
			{ U"nvgt", U"\u003E\u20D2" },
			{ U"fjlig", U"\u0066\u006A" },
			{ U"ThickSpace", U"\u205F\u200A" },
			{ U"nrarrw", U"\u219D\u0338" },
			{ U"npart", U"\u2202\u0338" },
			{ U"nang", U"\u2220\u20D2" },
			{ U"caps", U"\u2229\uFE00" },
			{ U"cups", U"\u222A\uFE00" },
			{ U"nvsim", U"\u223C\u20D2" },
			{ U"race", U"\u223D\u0331" },
			{ U"acE", U"\u223E\u0333" },
			{ U"nesim", U"\u2242\u0338" },
			{ U"NotEqualTilde", U"\u2242\u0338" },
			{ U"napid", U"\u224B\u0338" },
			{ U"nvap", U"\u224D\u20D2" },
			{ U"nbump", U"\u224E\u0338" },
			{ U"NotHumpDownHump", U"\u224E\u0338" },
			{ U"nbumpe", U"\u224F\u0338" },
			{ U"NotHumpEqual", U"\u224F\u0338" },
			{ U"nedot", U"\u2250\u0338" },
			{ U"bnequiv", U"\u2261\u20E5" },
			{ U"nvle", U"\u2264\u20D2" },
			{ U"nvge", U"\u2265\u20D2" },
			{ U"nlE", U"\u2266\u0338" },
			{ U"nleqq", U"\u2266\u0338" },
			{ U"ngE", U"\u2267\u0338" },
			{ U"ngeqq", U"\u2267\u0338" },
			{ U"NotGreaterFullEqual", U"\u2267\u0338" },
			{ U"lvertneqq", U"\u2268\uFE00" },
			{ U"lvnE", U"\u2268\uFE00" },
			{ U"gvertneqq", U"\u2269\uFE00" },
			{ U"gvnE", U"\u2269\uFE00" },
			{ U"nLtv", U"\u226A\u0338" },
			{ U"NotLessLess", U"\u226A\u0338" },
			{ U"nLt", U"\u226A\u20D2" },
			{ U"nGtv", U"\u226B\u0338" },
			{ U"NotGreaterGreater", U"\u226B\u0338" },
			{ U"nGt", U"\u226B\u20D2" },
			{ U"NotSucceedsTilde", U"\u227F\u0338" },
			{ U"NotSubset", U"\u2282\u20D2" },
			{ U"nsubset", U"\u2282\u20D2" },
			{ U"vnsub", U"\u2282\u20D2" },
			{ U"NotSuperset", U"\u2283\u20D2" },
			{ U"nsupset", U"\u2283\u20D2" },
			{ U"vnsup", U"\u2283\u20D2" },
			{ U"varsubsetneq", U"\u228A\uFE00" },
			{ U"vsubne", U"\u228A\uFE00" },
			{ U"varsupsetneq", U"\u228B\uFE00" },
			{ U"vsupne", U"\u228B\uFE00" },
			{ U"NotSquareSubset", U"\u228F\u0338" },
			{ U"NotSquareSuperset", U"\u2290\u0338" },
			{ U"sqcaps", U"\u2293\uFE00" },
			{ U"sqcups", U"\u2294\uFE00" },
			{ U"nvltrie", U"\u22B4\u20D2" },
			{ U"nvrtrie", U"\u22B5\u20D2" },
			{ U"nLl", U"\u22D8\u0338" },
			{ U"nGg", U"\u22D9\u0338" },
			{ U"lesg", U"\u22DA\uFE00" },
			{ U"gesl", U"\u22DB\uFE00" },
			{ U"notindot", U"\u22F5\u0338" },
			{ U"notinE", U"\u22F9\u0338" },
			{ U"nrarrc", U"\u2933\u0338" },
			{ U"NotLeftTriangleBar", U"\u29CF\u0338" },
			{ U"NotRightTriangleBar", U"\u29D0\u0338" },
			{ U"ncongdot", U"\u2A6D\u0338" },
			{ U"napE", U"\u2A70\u0338" },
			{ U"nleqslant", U"\u2A7D\u0338" },
			{ U"nles", U"\u2A7D\u0338" },
			{ U"NotLessSlantEqual", U"\u2A7D\u0338" },
			{ U"ngeqslant", U"\u2A7E\u0338" },
			{ U"nges", U"\u2A7E\u0338" },
			{ U"NotGreaterSlantEqual", U"\u2A7E\u0338" },
			{ U"NotNestedLessLess", U"\u2AA1\u0338" },
			{ U"NotNestedGreaterGreater", U"\u2AA2\u0338" },
			{ U"smtes", U"\u2AAC\uFE00" },
			{ U"lates", U"\u2AAD\uFE00" },
			{ U"NotPrecedesEqual", U"\u2AAF\u0338" },
			{ U"npre", U"\u2AAF\u0338" },
			{ U"npreceq", U"\u2AAF\u0338" },
			{ U"NotSucceedsEqual", U"\u2AB0\u0338" },
			{ U"nsce", U"\u2AB0\u0338" },
			{ U"nsucceq", U"\u2AB0\u0338" },
			{ U"nsubE", U"\u2AC5\u0338" },
			{ U"nsubseteqq", U"\u2AC5\u0338" },
			{ U"nsupE", U"\u2AC6\u0338" },
			{ U"nsupseteqq", U"\u2AC6\u0338" },
			{ U"varsubsetneqq", U"\u2ACB\uFE00" },
			{ U"vsubnE", U"\u2ACB\uFE00" },
			{ U"varsupsetneqq", U"\u2ACC\uFE00" },
			{ U"vsupnE", U"\u2ACC\uFE00" },
			{ U"nparsl", U"\u2AFD\u20E5" }
		};
	};

}

#endif
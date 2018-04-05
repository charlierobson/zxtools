#ifndef __argcrack_h
#define __argcrack_h

#include <string>
#include <complex>

// (C) 2009 Charlie Robson

class pathutil
{
    public:
      static bool changeextension(std::string &filename, const char *newextension)
      {
            size_t expos = filename.find_last_of('.');
            bool dotfound = expos != std::string::npos;
            if (dotfound)
            {
                  std::string base = filename.substr(0, expos + 1);

                  if (newextension[0] == '.')
                  {
                        ++newextension;
                  }

                  filename = base + newextension;
            }

            return dotfound;
      }

    private:
      pathutil(){};
      ~pathutil(){};
};

class argcrack
{
    public:
      argcrack(int argc, char **argv) : m_argc(argc),
                                        m_argv(argv)
      {
      }

      bool helprequested(void)
      {
            for (int i = 1; i < m_argc; ++i)
            {
                  const char *argp = m_argv[i];
                  while (*argp == '\\' || *argp == '/' || *argp == '-')
                  {
                        ++argp;
                  }

                  if (strcasecmp(argp, "?") == 0 ||
                      strcasecmp(argp, "help") == 0)
                  {
                        return true;
                  }
            }

            return false;
      }

      bool split(const int index, std::string &left, std::string &right)
      {
            if (m_argc > index + 1)
            {
                  std::string ssarg(m_argv[index + 1]);
                  size_t equoffs = ssarg.find_first_of('=');
                  if (equoffs != -1)
                  {
                        left = ssarg.substr(0, equoffs);
                        right = ssarg.substr(equoffs + 1, ssarg.length());
                        return true;
                  }
            }

            return false;
      }

      bool eval(const char *val, int &result)
      {
            int len = strlen(val);
            if (len)
            {
                  int base = 0;
                  if (*val == '%')
                  {
                        base = 2;
                        ++val;
                  }
                  else if (*val == '$')
                  {
                        base = 16;
                        ++val;
                  }
                  else if (len > 2 && val[0] == '0' && val[1] == 'x')
                  {
                        base = 16;
                        val += 2;
                  }

                  char *final;
                  result = strtol(val, &final, base);

                  if (*final == 0)
                  {
                        return true;
                  }
            }

            return false;
      }

      // pname should be of the form:
      //
      //  parm=
      //
      bool getint(const char *pname, int &target)
      {
            for (int i = 1; i < m_argc; ++i)
            {
                  if (strncasecmp(pname, m_argv[i], strlen(pname)) == 0)
                  {
                        eval(&m_argv[i][strlen(pname)], target);

                        return true;
                  }
            }

            return false;
      }

      // pname should be of the form:
      //
      //  parm=
      //
      bool getstring(const char *pname, std::string &target)
      {
            for (int i = 1; i < m_argc; ++i)
            {
                  if (strncasecmp(pname, m_argv[i], strlen(pname)) == 0)
                  {
                        target = &m_argv[i][strlen(pname)];
                        return true;
                  }
            }

            return false;
      }

      bool getat(int index, std::string &target)
      {
            bool enoughargs = m_argc > index;
            if (enoughargs)
            {
                  target = m_argv[index];
            }

            return enoughargs;
      }

      bool ispresent(const char *pname)
      {
            for (int i = 1; i < m_argc; ++i)
            {
                  if (strncasecmp(pname, m_argv[i], strlen(pname)) == 0)
                  {
                        return true;
                  }
            }

            return false;
      }

    private:
      int m_argc;
      char **m_argv;
};

#endif

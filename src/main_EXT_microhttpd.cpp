/*    This file is part of EXT_microhttpd
      The aim of this software is to provide way to use microhttpd library
      with my own build system
      Copyright (C) 2017  Julien Thevenon ( julien_thevenon at yahoo.fr )

      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifdef EXT_MICROHTTPD_SELF_TEST
#include <iostream>
#include <cstring>
#include <microhttpd.h>

int print_out_key (void *cls, enum MHD_ValueKind kind, const char *key,const char *value)
{
    (void)cls;
    (void)kind;
  std::cout << "\"" << key << "\" = \"" <<  value << "\"" << std::endl ;
  return MHD_YES;
}

int ahc_echo (void *p_callback_data,
          struct MHD_Connection *connection,
          const char *url,
          const char *method,
          const char *version,
          const char *upload_data, size_t * upload_data_size, void **ptr)
{
  (void)version;
  (void)upload_data;
  (void)upload_data_size;
  static int aptr;
  const char *p_content_page = (char*)p_callback_data;
  struct MHD_Response *response;
  int ret;

  std::cout << "Method = \"" << method << "\"" <<std::endl ;
  std::cout << "URL = \"" << url << "\"" <<std::endl ;

  MHD_get_connection_values (connection,
                             MHD_HEADER_KIND,
                             print_out_key,
                             nullptr);

  if (0 != strcmp (method, "GET"))
    return MHD_NO;              /* unexpected method */
  if (&aptr != *ptr)
    {
      /* do never respond on first call */
      *ptr = &aptr;
      return MHD_YES;
    }
  *ptr = nullptr;                  /* reset when done */
  response = MHD_create_response_from_buffer (strlen (p_content_page),
                                              (void *) p_content_page,
                                              MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);
  return ret;
}


int main(int argc, char ** argv)
{
  std::cout << "Test of lib GNU microhttpd" << std::endl ;
  struct MHD_Daemon *d;

  if (argc != 2)
    {
      std::cout << argv[0] << "PORT" << std::endl;
      return 1;
    }

    //#define PAGE "<html><head><title>libmicrohttpd demo</title></head><body>libmicrohttpd demo</body></html>"
    std::string page = "<html><head><title>libmicrohttpd demo</title></head><body>libmicrohttpd demo<br>\n<form name=\"Show\">\n<input type=\"text\" name=\"MouseX\" value=\"0\" size=\"4\"> X<br>\n<input type=\"text\" name=\"MouseY\" value=\"0\" size=\"4\"> Y<br>\n</form>\n\n<script language=\"JavaScript1.2\">\n<!--\n\n// Detect if the browser is IE or not.\n// If it is not IE, we assume that the browser is NS.\nvar IE = document.all?true:false\n\n// If NS -- that is, !IE -- then set up for mouse capture\nif (!IE) document.captureEvents(Event.MOUSEMOVE)\n\n// Set-up to use getMouseXY function onMouseMove\ndocument.onmousemove = getMouseXY;\n\n// Temporary variables to hold mouse x-y pos.s\nvar tempX = 0\nvar tempY = 0\n\n// Main function to retrieve mouse x-y pos.s\n\nfunction getMouseXY(e) {\n  if (IE) { // grab the x-y pos.s if browser is IE\n    tempX = event.clientX + document.body.scrollLeft\n    tempY = event.clientY + document.body.scrollTop\n  } else {  // grab the x-y pos.s if browser is NS\n    tempX = e.pageX\n    tempY = e.pageY\n  }  \n  // catch possible negative values in NS4\n  if (tempX < 0){tempX = 0}\n  if (tempY < 0){tempY = 0}  \n  // show the position values in the form named Show\n  // in the text fields named MouseX and MouseY\n  document.Show.MouseX.value = tempX\n  document.Show.MouseY.value = tempY\n  return true\n}\n\n//-->\n</script>\n<TABLE BORDER>\n<CAPTION>l�gende en haut</CAPTION>\n<TR><TD><IMG src=\"toto.bmp\"></TD></TR>\n<TR><TD>deuxi�me ligne</TD></TR>\n</TABLE>\n</body></html>";

    d = MHD_start_daemon (// MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG | MHD_USE_POLL,
                        MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG,
                        // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG | MHD_USE_POLL,
                        // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG,
                        (uint16_t)std::stoi(argv[1]),
                        nullptr,
                        nullptr,
                        &ahc_echo,
                        (void*)page.c_str(),
                        MHD_OPTION_CONNECTION_TIMEOUT,
                        (unsigned int) 120,
                        MHD_OPTION_END);
  if (d == nullptr)
    return 1;
  (void) getc (stdin);
  MHD_stop_daemon (d);
  return 0;
}
#endif // EXT_MICROHTTPD_SELF_TEST
//EOF

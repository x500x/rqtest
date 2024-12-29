  foreach(z_vcpkg_curl_component IN ITEMS alt-svc AsynchDNS brotli HSTS HTTP2 HTTPS-proxy IPv6 Kerberos Largefile libz NTLM SPNEGO SSL SSPI threadsafe Unicode UnixSockets zstd DICT FILE FTP FTPS GOPHER GOPHERS HTTP HTTPS IMAP IMAPS IPFS IPNS LDAP LDAPS MQTT POP3 POP3S RTSP SMB SMBS SMTP SMTPS TELNET TFTP WS WSS)
    if(z_vcpkg_curl_component MATCHES "^[-_a-zA-Z0-9]*$")
      set(CURL_${z_vcpkg_curl_component}_FOUND TRUE)
    endif()
  endforeach()
  
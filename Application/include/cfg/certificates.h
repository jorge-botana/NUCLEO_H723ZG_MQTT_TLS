// ------------------------------------------------------------------------------
// CERTIFICATES
//
// - Certificados y par de claves para TLS_CLIENT:
//
//   -> Certificado del CA raíz de JORGE-PC
//   -> Certificado del cliente de JORGE-PC
//   -> Certificado del CA raíz de test.mosquitto.org
//   -> Certificado del cliente de test.mosquitto.org
//   -> Par de claves del cliente.
//
// ------------------------------------------------------------------------------
// Copyright (c) 2021 Jorge Botana Mtz. de Ibarreta
//
// Este archivo se encuentra bajo los términos de la Licencia MIT. Debería
// haberse proporcionado una copia de ella junto a este fichero. Si no es así, se
// puede encontrar en el siguiente enlace:
//
//                                            https://opensource.org/licenses/MIT
// ------------------------------------------------------------------------------

#ifndef   CERTIFICATES_H
#define   CERTIFICATES_H

// ------------------------------------------------------------------------------

#include "setup.h"

#ifdef    USE_MAIN_MQTT_BROKER

#define   CA_CRT                                                  \
"\
-----BEGIN CERTIFICATE-----\n\
MIIC8jCCAdqgAwIBAgIBATANBgkqhkiG9w0BAQsFADASMRAwDgYDVQQDDAdSb290\n\
IENBMB4XDTAwMDEwMTAwMDAwMFoXDTI5MTIzMTIzNTk1OVowEjEQMA4GA1UEAwwH\n\
Um9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL2Y/0Kqgrw+\n\
ypzuRCRn0GjPd1RPmDTVaw764zFjjAuCRSA2X5wzr27uIstRnOUbc2aLmcGH3cgk\n\
Hsn8yWUbTslhzK45SgejucJtnG8EuRm3LdLRvKh2cmPSHeCTX0uNk9sR0a6oy5S+\n\
F/Mv7yRfA2fiaizIajb6GpSeZ91sTshbNlLiql7vCUCJpJ1fJrUCHGM9NyOMe7Ra\n\
DgzJ2gbl7pDRIqzB/95wf4Zsfqz6P7avYHjzavOKZm0HGOO/drT5tBSWN5ovT/p4\n\
NgtuBfGjKXYoUeWXeq8T9zfSdpaezGcrEr0NJRTvauNrNZrfVAApzA2rtLsndynt\n\
xMakXOOUZWsCAwEAAaNTMFEwDwYDVR0TBAgwBgEB/wIBATAdBgNVHQ4EFgQU6snm\n\
9H6ertcqFio83qZ1PlhVeU0wHwYDVR0jBBgwFoAU6snm9H6ertcqFio83qZ1PlhV\n\
eU0wDQYJKoZIhvcNAQELBQADggEBAHaoVXSPOYB/j1WrjonMR192ZrKU44CsN7C2\n\
JArGiNhErOkRpf2t57UGTHF18gsrwBqI8cQ/hM6ij7Ewyvl9Ngsn+NvRy7iJIl3g\n\
fxzwH9bRiTCeo2vbqpIYYMYYfV++Od43xDEbOhU+zZQ1FZSRaC+kBszDQz7nmnQE\n\
luiDKbAThQipqQfTTIdswaZQCyOtczhoq7vuI117xvtSrjE2e0kkcOj9DdtaJJ8U\n\
DWODK6XGQJ4xlEScyhfgZjfjD232cHZV2sf5h6LDIpmBIEu/KlYoITuVz/yRubR/\n\
BtGp8avVp0obxQm2hJpW0IzdJBNt2VADL28qvHmlq/ehPtZYQ2g=\n\
-----END CERTIFICATE-----\n\
"

#define   CLIENT_CRT                                              \
"\
-----BEGIN CERTIFICATE-----\n\
MIIC8jCCAdqgAwIBAgIBATANBgkqhkiG9w0BAQsFADASMRAwDgYDVQQDDAdNUVRU\n\
IENBMB4XDTIxMDEwMTAwMDAwMFoXDTIzMTIzMTIzNTk1OVowGDEWMBQGA1UEAwwN\n\
TlVDTEVPLUg3MjNaRzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANlG\n\
LE9mF67UzARAgbRTMwqIONFlD5SmKJwJtcOeiMKEUoND41CPmd/jHp8PzbJKpCeD\n\
SA0fcr62CvkXMM/Hu2JvE+3CPHXjEJz5kPMk4nZ0v/vOr3aBj1FrMwQtw4vq8atv\n\
PH10pZ2g5/9fNHoEjO17RXyGUYGmXdA+6KFRNyB7AVqxyXmBbqfkaJ6YeY4pAANI\n\
3dARXtDXSK2e/Dts7Ra6X1em7gS8ObdQnoK1ipv9kjMLFbnKCjXV8seAYr7oUUdE\n\
QhdW21IWq3IanQP2vaA+KrgQqvQpJpUplHqIkzjxTOzsEN9DRn0sTlwli52IrWAJ\n\
73sKRrIYHJ0hlCvTAFkCAwEAAaNNMEswCQYDVR0TBAIwADAdBgNVHQ4EFgQU33lS\n\
Nf29lzhL+oldUG5Suj0kzXAwHwYDVR0jBBgwFoAUnQO2I0uPXS60oRliRvBD8vX4\n\
4RUwDQYJKoZIhvcNAQELBQADggEBAJbgc0x0/lCz6rwpTWi66jMV8aIL+iF9jHgn\n\
ycTZXuWuQjZ9DHoPTxIbPTrSgnsoffblKB0uPEzPfix23EIXTBsJ62a0lirZEiHm\n\
iiqdvu/M/+LuZr1dhUh0stxN7KzPLev0A3VM84bH/Lkfbvva9Qw8SZYH4r/AyoGD\n\
8m2y2zwBN3LmTKdVqddWDTEs1z7mJKJx3tg/oxqce/R0w7kZDvg5aIIrrWyaboUF\n\
vegOv2Qj7SE7GTYMyRaI+kDTcA+KCWNEIfnT2WRfrhwPEyB+kP7iVLC5V0Jb554L\n\
15udqxW134CDz9W95r/H1rR4q0sL4oBPnwMRvsf73WJgqChYydA=\n\
-----END CERTIFICATE-----\n\
"

#else  // USE_MAIN_MQTT_BROKER

#define   CA_CRT                                                  \
"\
-----BEGIN CERTIFICATE-----\n\
MIIEAzCCAuugAwIBAgIUBY1hlCGvdj4NhBXkZ/uLUZNILAwwDQYJKoZIhvcNAQEL\n\
BQAwgZAxCzAJBgNVBAYTAkdCMRcwFQYDVQQIDA5Vbml0ZWQgS2luZ2RvbTEOMAwG\n\
A1UEBwwFRGVyYnkxEjAQBgNVBAoMCU1vc3F1aXR0bzELMAkGA1UECwwCQ0ExFjAU\n\
BgNVBAMMDW1vc3F1aXR0by5vcmcxHzAdBgkqhkiG9w0BCQEWEHJvZ2VyQGF0Y2hv\n\
by5vcmcwHhcNMjAwNjA5MTEwNjM5WhcNMzAwNjA3MTEwNjM5WjCBkDELMAkGA1UE\n\
BhMCR0IxFzAVBgNVBAgMDlVuaXRlZCBLaW5nZG9tMQ4wDAYDVQQHDAVEZXJieTES\n\
MBAGA1UECgwJTW9zcXVpdHRvMQswCQYDVQQLDAJDQTEWMBQGA1UEAwwNbW9zcXVp\n\
dHRvLm9yZzEfMB0GCSqGSIb3DQEJARYQcm9nZXJAYXRjaG9vLm9yZzCCASIwDQYJ\n\
KoZIhvcNAQEBBQADggEPADCCAQoCggEBAME0HKmIzfTOwkKLT3THHe+ObdizamPg\n\
UZmD64Tf3zJdNeYGYn4CEXbyP6fy3tWc8S2boW6dzrH8SdFf9uo320GJA9B7U1FW\n\
Te3xda/Lm3JFfaHjkWw7jBwcauQZjpGINHapHRlpiCZsquAthOgxW9SgDgYlGzEA\n\
s06pkEFiMw+qDfLo/sxFKB6vQlFekMeCymjLCbNwPJyqyhFmPWwio/PDMruBTzPH\n\
3cioBnrJWKXc3OjXdLGFJOfj7pP0j/dr2LH72eSvv3PQQFl90CZPFhrCUcRHSSxo\n\
E6yjGOdnz7f6PveLIB574kQORwt8ePn0yidrTC1ictikED3nHYhMUOUCAwEAAaNT\n\
MFEwHQYDVR0OBBYEFPVV6xBUFPiGKDyo5V3+Hbh4N9YSMB8GA1UdIwQYMBaAFPVV\n\
6xBUFPiGKDyo5V3+Hbh4N9YSMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEL\n\
BQADggEBAGa9kS21N70ThM6/Hj9D7mbVxKLBjVWe2TPsGfbl3rEDfZ+OKRZ2j6AC\n\
6r7jb4TZO3dzF2p6dgbrlU71Y/4K0TdzIjRj3cQ3KSm41JvUQ0hZ/c04iGDg/xWf\n\
+pp58nfPAYwuerruPNWmlStWAXf0UTqRtg4hQDWBuUFDJTuWuuBvEXudz74eh/wK\n\
sMwfu1HFvjy5Z0iMDU8PUDepjVolOCue9ashlS4EB5IECdSR2TItnAIiIwimx839\n\
LdUdRudafMu5T5Xma182OC0/u/xRlEm+tvKGGmfFcN0piqVl8OrSPBgIlb+1IKJE\n\
m/XriWr/Cq4h/JfB7NTsezVslgkBaoU=\n\
-----END CERTIFICATE-----\n\
"

#define   CLIENT_CRT                                              \
"\
-----BEGIN CERTIFICATE-----\n\
MIIDaDCCAlCgAwIBAgIBADANBgkqhkiG9w0BAQsFADCBkDELMAkGA1UEBhMCR0Ix\n\
FzAVBgNVBAgMDlVuaXRlZCBLaW5nZG9tMQ4wDAYDVQQHDAVEZXJieTESMBAGA1UE\n\
CgwJTW9zcXVpdHRvMQswCQYDVQQLDAJDQTEWMBQGA1UEAwwNbW9zcXVpdHRvLm9y\n\
ZzEfMB0GCSqGSIb3DQEJARYQcm9nZXJAYXRjaG9vLm9yZzAeFw0yMTExMDIyMDE0\n\
NDlaFw0yMjAxMzEyMDE0NDlaMEIxFjAUBgNVBAMMDU5VQ0xFTy1INzIzWkcxGzAZ\n\
BgNVBAoMElRGRyAtIEpvcmdlIEJvdGFuYTELMAkGA1UEBhMCRVMwggEiMA0GCSqG\n\
SIb3DQEBAQUAA4IBDwAwggEKAoIBAQDZRixPZheu1MwEQIG0UzMKiDjRZQ+Upiic\n\
CbXDnojChFKDQ+NQj5nf4x6fD82ySqQng0gNH3K+tgr5FzDPx7tibxPtwjx14xCc\n\
+ZDzJOJ2dL/7zq92gY9RazMELcOL6vGrbzx9dKWdoOf/XzR6BIzte0V8hlGBpl3Q\n\
PuihUTcgewFascl5gW6n5GiemHmOKQADSN3QEV7Q10itnvw7bO0Wul9Xpu4EvDm3\n\
UJ6CtYqb/ZIzCxW5ygo11fLHgGK+6FFHREIXVttSFqtyGp0D9r2gPiq4EKr0KSaV\n\
KZR6iJM48Uzs7BDfQ0Z9LE5cJYudiK1gCe97CkayGBydIZQr0wBZAgMBAAGjGjAY\n\
MAkGA1UdEwQCMAAwCwYDVR0PBAQDAgXgMA0GCSqGSIb3DQEBCwUAA4IBAQBaDJjS\n\
/yGyypwb87wTp6QWivEl8FLrw3WdJokSXRxlxeXZqZfKeyat4G6EneQSRPsiAhQB\n\
POnt1/6IauYeoGg1AtnVDB8abSdEbuLEUcYMzrnGZJ2uYLHh3C+nUHptS12KYU7C\n\
yspMgp6LgnJBNyi8fD2GSSdhNCKHnYEKCKgJg6Ch4hLFgQCHLlHxwHktEW7zpmMK\n\
CO1iFTwVihv4Z89NXbXlr4cXCwcBuc/9tQNFdb/Wl1f9pX3zMFnkO53i/GT+1TFa\n\
sRYPuyZmSZsqI4kRQdSRcBcI6sWzjlkNMBw0i5vH2q9aQ2cH7AkeygEH1pLSsS/n\n\
bnHVWgpqe6KtORd7\n\
-----END CERTIFICATE-----\n\
"

#endif // USE_MAIN_MQTT_BROKER

#define   CLIENT_KEY                                              \
"\
-----BEGIN RSA PRIVATE KEY-----\n\
MIIEpAIBAAKCAQEA2UYsT2YXrtTMBECBtFMzCog40WUPlKYonAm1w56IwoRSg0Pj\n\
UI+Z3+Menw/NskqkJ4NIDR9yvrYK+Rcwz8e7Ym8T7cI8deMQnPmQ8yTidnS/+86v\n\
doGPUWszBC3Di+rxq288fXSlnaDn/180egSM7XtFfIZRgaZd0D7ooVE3IHsBWrHJ\n\
eYFup+Ronph5jikAA0jd0BFe0NdIrZ78O2ztFrpfV6buBLw5t1CegrWKm/2SMwsV\n\
ucoKNdXyx4BivuhRR0RCF1bbUharchqdA/a9oD4quBCq9CkmlSmUeoiTOPFM7OwQ\n\
30NGfSxOXCWLnYitYAnvewpGshgcnSGUK9MAWQIDAQABAoIBAGE3REE8eXFJFoX6\n\
hO514ZsfS2jpsph85iO3NoRyLVf3k+HW4OFdd2iVO1DJZeyV+DATzgvNG+7YDj3Y\n\
0gbOI/pNoBmcdKf8sqTCB8i7IrcE0xfsqVa5brJr6DZA8cNrmrltup+ZrzbB1bxO\n\
UXEdEx95LiSjBtofYc95LmZrq+JUQ/DEGAQB0YDoS0pcs7IOA4TTxaMUSD5+sWVr\n\
2kg6jKCCLKzFlV9y3BGofhuvwV1MqT5fOZO8/CD27z1fiRvanotxT3Q+wz4g4lC0\n\
TwQ6yzverdux/Qv0ZrIoYWA9m6IDO8pWTp2ZpYuZG6ypZ+MwOzhgbbm3EGKw7Pio\n\
2lVV5RcCgYEA+PeBBar5DB8DEgKn8GaPuiHZvHJS81TuOdcdWL0qY382bvA/agBu\n\
2udfMSfSIVDfQvzt7m9nCJ3VgK3uqNGMayWqnugJg1LiiDBfSeSoggWFgr2ZOSTj\n\
mAuqP0D6fJzpXrEL9jVEnKOcaioYX/ECvq0aidXX1o2M5Iriyqp8t78CgYEA32l4\n\
uJin4QZKkySaKbxYrXYg8Wddfo6dic99UL9m2hwnsJhp10147Nf/A24R3CFHWGiK\n\
L3U/QvdHOVi2XfkTyLW+gYeE1lz0x17imZVZKLO6z38pnxf8afLFFAN3m/Hc9BvT\n\
dFrEFAUg+MmmkYl844O/Qx/1ZCml9U1NXvkrjecCgYEAihbNi1U/eYeh7lVmzKI2\n\
SekImctXUvviflZgQp35auJeUL4UjjjbQ7NdSyhpFm2Cx+b+GWfU4CSbDRpr61r+\n\
5VcpAaZ10E8isqMR1yxqgmOJn/CFkfpx+h1OOnNR+9gUYJ8WoWL+QVtGIGRkzKIj\n\
AZuFwrnHU4uDgCfAjuRs9IUCgYAUrpvpYuBvuXhf5MyMvo3rCPuFvY7vL16wKlAs\n\
kHuCB4c5FvL9ZoOiUWCfPapTmZUUPyFDwXPozCGA0pCXZH6LXx/St+v5OEPEzSjn\n\
FOdbGEBcAyakPsE0QbAy6RtCiYJt6sGJjPmH7O2fenN8F6FDIqgrG6NI8X4WlmBq\n\
QWDh0wKBgQCa/WZI3vXp5Pc6lux7ywc0jMd7S4sVI/LsnUXpOFv0/T1B0qR8mMiv\n\
1bjQ+U6yUKMYS6gkdhwos76gTS8SVtZvikTAL2ZxI6Pd04twoTwL9TFAUzyX5fhU\n\
ZkWDl/BfJcczve76qE0tJ1sqXRTnyfpLkDAKEp2kPDeYIqZ2Hfujnw==\n\
-----END RSA PRIVATE KEY-----\n\
"

// ------------------------------------------------------------------------------

#endif // CERTIFICATES_H

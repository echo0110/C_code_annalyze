----------------------------------
Testing of Crypto Algorithms
----------------------------------

For testing the Crypto Algorithm, go in test folder
and make.

The complete testing can be done by executing the following command:
./Auto_Run.txt

For individual testing following should be done:

-----------
Testing AES
-----------
From test folder, give the following command:
./sanity < aes.dat

aes.dat file contains test vectors for AES in the following format
cbc:<block-cipher-name>:<key>:<initialization-vector>:<block>:<expected-encrypted-block>

For the testing API functions, the following command is given in lib folder:
./AesArApiTest

-----------
Testing DES
-----------
From lib folder, give the following command:
./sanity < des.dat

des.dat file contains test vectors for AES in the following format
cbc:<block-cipher-name>:<key>:<initialization-vector>:<block>:<expected-encrypted-block>

For the testing API functions, the following command is given in test folder:
./DesArApiTest

------------
Testing SHA1
------------
From lib folder, give the following command:
./sanity < sha1.dat

sha1.dat file contains test vectors for SHA1 in the following format
HASH:<hash-function-name>:<block>:<expected-hash>

For the testing of the API functions, the following
command is given in test folder:
./convert hex <data> | ./ShaArApiTest hash sha1

------------
Testing HMAC
------------
From lib folder, give the following command:
./sanity < hmac_md5.dat

------------
Testing MD5
------------
From lib folder, give the following command:
./sanity < md5.dat


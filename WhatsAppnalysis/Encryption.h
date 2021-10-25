#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#include <conio.h>
#include "NewDefinitions.h"

// Link with the Advapi32.lib file.
#pragma comment (lib, "advapi32")


bool MyEncryptFile(
    LPCWSTR szSource,
    LPCWSTR szDestination,
    LPCWSTR szPassword);

void MyHandleErrorE(
    std::string psz,
    int nErrorNumber);

//Encrypts a file
bool encrypt(LPCWSTR pszSource,
    LPCWSTR pszDestination, LPCWSTR password)
{
    
    //---------------------------------------------------------------
    // Call EncryptFile to do the actual encryption.
    if (MyEncryptFile(pszSource, pszDestination, password))
    {
        return true;
    }
    else
    {
        MyHandleErrorE(
            ("Error encrypting file!\n"),
            GetLastError());
        return false;
    }
}

//-------------------------------------------------------------------
// Code for the function MyEncryptFile called by main.
//-------------------------------------------------------------------
// Parameters passed are:
//  pszSource, the name of the input, a plaintext file.
//  pszDestination, the name of the output, an encrypted file to be 
//   created.
//  pszPassword, either NULL if a password is not to be used or the 
//   string that is the password.
bool MyEncryptFile(
    LPCWSTR pszSourceFile,
    LPCWSTR pszDestinationFile,
    LPCWSTR pszPassword){
    bool fEOF = false;
    bool fReturn = false;
    HANDLE hSourceFile = INVALID_HANDLE_VALUE;
    HANDLE hDestinationFile = INVALID_HANDLE_VALUE;

    HCRYPTPROV hCryptProv = NULL;
    HCRYPTKEY hKey = NULL;
    HCRYPTKEY hXchgKey = NULL;
    HCRYPTHASH hHash = NULL;

    PBYTE pbKeyBlob = NULL;
    DWORD dwKeyBlobLen;

    PBYTE pbBuffer = NULL;
    DWORD dwBlockLen;
    DWORD dwBufferLen;
    DWORD dwCount;

    //---------------------------------------------------------------
    // Open the source file. 
    hSourceFile = CreateFile(
        pszSourceFile,
        FILE_READ_DATA,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (INVALID_HANDLE_VALUE != hSourceFile)
    {
    }
    else
    {
        MyHandleErrorE(
            ("Error opening source plaintext file!\n"),
            GetLastError());
        goto Exit_MyEncryptFile;
    }

    //---------------------------------------------------------------
    // Open the destination file. 
    hDestinationFile = CreateFile(
        pszDestinationFile,
        FILE_WRITE_DATA,
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (INVALID_HANDLE_VALUE != hDestinationFile)
    {
    }
    else
    {
        MyHandleErrorE(
            ("Error opening destination file!\n"),
            GetLastError());
        goto Exit_MyEncryptFile;
    }

    //---------------------------------------------------------------
    // Get the handle to the default provider. 
    if (CryptAcquireContext(
        &hCryptProv,
        NULL,
        MS_ENHANCED_PROV,
        PROV_RSA_FULL,
        0))
    {
    }
    else
    {
        MyHandleErrorE(
            ("Error during CryptAcquireContext!\n"),
            GetLastError());
        goto Exit_MyEncryptFile;
    }

    //---------------------------------------------------------------
    // Create the session key.
    if (!pszPassword || !pszPassword[0])
    {
        //-----------------------------------------------------------
        // No password was passed.
        // Encrypt the file with a random session key, and write the 
        // key to a file. 

        //-----------------------------------------------------------
        // Create a random session key. 
        if (CryptGenKey(
            hCryptProv,
            ENCRYPT_ALGORITHM,
            KEYLENGTH | CRYPT_EXPORTABLE,
            &hKey))
        {
        }
        else
        {
            MyHandleErrorE(
                ("Error during CryptGenKey. \n"),
                GetLastError());
            goto Exit_MyEncryptFile;
        }

        //-----------------------------------------------------------
        // Get the handle to the exchange public key. 
        if (CryptGetUserKey(
            hCryptProv,
            AT_KEYEXCHANGE,
            &hXchgKey))
        {
        }
        else
        {
            if (NTE_NO_KEY == GetLastError())
            {
                // No exchange key exists. Try to create one.
                if (!CryptGenKey(
                    hCryptProv,
                    AT_KEYEXCHANGE,
                    CRYPT_EXPORTABLE,
                    &hXchgKey))
                {
                    MyHandleErrorE(
                        ("Could not create "
                            "a user public key.\n"),
                        GetLastError());
                    goto Exit_MyEncryptFile;
                }
            }
            else
            {
                MyHandleErrorE(
                    ("User public key is not available and may not exist.\n"),
                    GetLastError());
                goto Exit_MyEncryptFile;
            }
        }

        //-----------------------------------------------------------
        // Determine size of the key BLOB, and allocate memory. 
        if (CryptExportKey(
            hKey,
            hXchgKey,
            SIMPLEBLOB,
            0,
            NULL,
            &dwKeyBlobLen))
        {
        }
        else
        {
            MyHandleErrorE(
                ("Error computing BLOB length! \n"),
                GetLastError());
            goto Exit_MyEncryptFile;
        }

        if (pbKeyBlob = (BYTE*)malloc(dwKeyBlobLen))
        {
        }
        else
        {
            MyHandleErrorE(("Out of memory. \n"), E_OUTOFMEMORY);
            goto Exit_MyEncryptFile;
        }

        //-----------------------------------------------------------
        // Encrypt and export the session key into a simple key 
        // BLOB. 
        if (CryptExportKey(
            hKey,
            hXchgKey,
            SIMPLEBLOB,
            0,
            pbKeyBlob,
            &dwKeyBlobLen))
        {
        }
        else
        {
            MyHandleErrorE(
                ("Error during CryptExportKey!\n"),
                GetLastError());
            goto Exit_MyEncryptFile;
        }

        //-----------------------------------------------------------
        // Release the key exchange key handle. 
        if (hXchgKey)
        {
            if (!(CryptDestroyKey(hXchgKey)))
            {
                MyHandleErrorE(
                    ("Error during CryptDestroyKey.\n"),
                    GetLastError());
                goto Exit_MyEncryptFile;
            }

            hXchgKey = 0;
        }

        //-----------------------------------------------------------
        // Write the size of the key BLOB to the destination file. 
        if (!WriteFile(
            hDestinationFile,
            &dwKeyBlobLen,
            sizeof(DWORD),
            &dwCount,
            NULL))
        {
            MyHandleErrorE(
                ("Error writing header.\n"),
                GetLastError());
            goto Exit_MyEncryptFile;
        }
        else
        {
        }

        //-----------------------------------------------------------
        // Write the key BLOB to the destination file. 
        if (!WriteFile(
            hDestinationFile,
            pbKeyBlob,
            dwKeyBlobLen,
            &dwCount,
            NULL))
        {
            MyHandleErrorE(
                ("Error writing header.\n"),
                GetLastError());
            goto Exit_MyEncryptFile;
        }
        else
        {
        }

        // Free memory.
        free(pbKeyBlob);
    }
    else
    {

        //-----------------------------------------------------------
        // The file will be encrypted with a session key derived 
        // from a password.
        // The session key will be recreated when the file is 
        // decrypted only if the password used to create the key is 
        // available. 

        //-----------------------------------------------------------
        // Create a hash object. 
        if (CryptCreateHash(
            hCryptProv,
            CALG_MD5,
            0,
            0,
            &hHash))
        {
        }
        else
        {
            MyHandleErrorE(
                ("Error during CryptCreateHash!\n"),
                GetLastError());
            goto Exit_MyEncryptFile;
        }

        //-----------------------------------------------------------
        // Hash the password. 
        if (CryptHashData(
            hHash,
            (BYTE*)pszPassword,
            lstrlen(pszPassword),
            0))
        {
        }
        else
        {
            MyHandleErrorE(
                ("Error during CryptHashData. \n"),
                GetLastError());
            goto Exit_MyEncryptFile;
        }

        //-----------------------------------------------------------
        // Derive a session key from the hash object. 
        if (CryptDeriveKey(
            hCryptProv,
            ENCRYPT_ALGORITHM,
            hHash,
            KEYLENGTH,
            &hKey))
        {
        }
        else
        {
            MyHandleErrorE(
                ("Error during CryptDeriveKey!\n"),
                GetLastError());
            goto Exit_MyEncryptFile;
        }
    }

    //---------------------------------------------------------------
    // The session key is now ready. If it is not a key derived from 
    // a  password, the session key encrypted with the private key 
    // has been written to the destination file.

    //---------------------------------------------------------------
    // Determine the number of bytes to encrypt at a time. 
    // This must be a multiple of ENCRYPT_BLOCK_SIZE.
    // ENCRYPT_BLOCK_SIZE is set by a #define statement.
    dwBlockLen = 1000 - 1000 % ENCRYPT_BLOCK_SIZE;

    //---------------------------------------------------------------
    // Determine the block size. If a block cipher is used, 
    // it must have room for an extra block. 
    if (ENCRYPT_BLOCK_SIZE > 1)
    {
        dwBufferLen = dwBlockLen + ENCRYPT_BLOCK_SIZE;
    }
    else
    {
        dwBufferLen = dwBlockLen;
    }

    //---------------------------------------------------------------
    // Allocate memory. 
    if (pbBuffer = (BYTE*)malloc(dwBufferLen))
    {
    }
    else
    {
        MyHandleErrorE(("Out of memory. \n"), E_OUTOFMEMORY);
        goto Exit_MyEncryptFile;
    }

    //---------------------------------------------------------------
    // In a do loop, encrypt the source file, 
    // and write to the source file. 
    do
    {
        //-----------------------------------------------------------
        // Read up to dwBlockLen bytes from the source file. 
        if (!ReadFile(
            hSourceFile,
            pbBuffer,
            dwBlockLen,
            &dwCount,
            NULL))
        {
            MyHandleErrorE(
                ("Error reading plaintext!\n"),
                GetLastError());
            goto Exit_MyEncryptFile;
        }

        if (dwCount < dwBlockLen)
        {
            fEOF = TRUE;
        }

        //-----------------------------------------------------------
        // Encrypt data. 
        if (!CryptEncrypt(
            hKey,
            NULL,
            fEOF,
            0,
            pbBuffer,
            &dwCount,
            dwBufferLen))
        {
            MyHandleErrorE(
                ("Error during CryptEncrypt. \n"),
                GetLastError());
            goto Exit_MyEncryptFile;
        }

        //-----------------------------------------------------------
        // Write the encrypted data to the destination file. 
        if (!WriteFile(
            hDestinationFile,
            pbBuffer,
            dwCount,
            &dwCount,
            NULL))
        {
            MyHandleErrorE(
                ("Error writing ciphertext.\n"),
                GetLastError());
            goto Exit_MyEncryptFile;
        }

        //-----------------------------------------------------------
        // End the do loop when the last block of the source file 
        // has been read, encrypted, and written to the destination 
        // file.
    } while (!fEOF);

    fReturn = true;

Exit_MyEncryptFile:
    //---------------------------------------------------------------
    // Close files.
    if (hSourceFile)
    {
        CloseHandle(hSourceFile);
    }

    if (hDestinationFile)
    {
        CloseHandle(hDestinationFile);
    }

    //---------------------------------------------------------------
    // Free memory. 
    if (pbBuffer)
    {
        free(pbBuffer);
    }


    //-----------------------------------------------------------
    // Release the hash object. 
    if (hHash)
    {
        if (!(CryptDestroyHash(hHash)))
        {
            MyHandleErrorE(
                ("Error during CryptDestroyHash.\n"),
                GetLastError());
        }

        hHash = NULL;
    }

    //---------------------------------------------------------------
    // Release the session key. 
    if (hKey)
    {
        if (!(CryptDestroyKey(hKey)))
        {
            MyHandleErrorE(
                ("Error during CryptDestroyKey!\n"),
                GetLastError());
        }
    }

    //---------------------------------------------------------------
    // Release the provider handle. 
    if (hCryptProv)
    {
        if (!(CryptReleaseContext(hCryptProv, 0)))
        {
            MyHandleErrorE(
                ("Error during CryptReleaseContext!\n"),
                GetLastError());
        }
    }

    return fReturn;
} // End Encryptfile.


//-------------------------------------------------------------------
//  This example uses the function MyHandleErrorE, a simple error
//  handling function, to print an error message to the  
//  standard error (stderr) file and exit the program. 
//  For most applications, replace this function with one 
//  that does more extensive error reporting.

void MyHandleErrorE(std::string psz, int nErrorNumber)
{
    //TestBox((psz) + itos(nErrorNumber));
}

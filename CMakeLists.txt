cmake_minimum_required(VERSION 3.10)
project(nuloan_project C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Find GTK4 using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

# Check if GTK4 was found
if (NOT GTK4_FOUND)
    message(FATAL_ERROR "GTK4 not found. Please install it.")
endif()

# Include GTK include directories
include_directories(${GTK4_INCLUDE_DIRS})

# Find libgcrypt
pkg_check_modules(GCRYPT REQUIRED libgcrypt)
include_directories(${GCRYPT_INCLUDE_DIRS})
link_directories(${GCRYPT_LIBRARY_DIRS})

# Find the CURL library
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

# Add your source files
set(SOURCE_FILES
        main.c
        Login.c
        Check_User_Information.c
        Check_User_Information.h
        SignUp.c
        SignUp.h
        Check_Signup_Information.c
        Check_Signup_Information.h
        Check_email.c
        Check_email.h
        Send_Email.c
        Send_Email.h
        hash_password.c
        hash_password.h
        Bank_information.c
        Bank_information.h
        Check_Bank_Information.c
        Check_Bank_Information.h
        Save_file.c
        Save_file.h
        Structures/User.h
        Structures/Bank_User_info.h
        Structures/Application.h
        Login_Process_Handle_Window.h
        Login_Process_Handle_Window.c
        Super_Admin.h
        Super_Admin.c
        Show_Users.c
        Show_Users.h
        Active_Member.c
        Active_Member.h
        Apply_Loan.c
        Apply_Loan.h
        Check_Loan_Status.c
        Check_Loan_Status.h
        First_Page.c
        First_Page.h
        Apply_Loan_More_Information.c
        Apply_Loan_More_Information.h
        Apply_Loan_fianl.c
        Apply_Loan_fianl.h
        Show_Loan_Application.c
        Show_Loan_Application.h
        Auto_Check_application.c
        Auto_Check_application.h
        Approve_Reject_Application.c
        Approve_Reject_Application.h
        Check_Loan_Status_more.c
        Check_Loan_Status_more.h
        Forget_Password.c
        Forget_Password.c

)

# Add your executable and link it with GTK4, libgcrypt, CURL, and regex libraries
add_executable(nuloan_project ${SOURCE_FILES})
target_link_libraries(nuloan_project ${GTK4_LIBRARIES} ${GCRYPT_LIBRARIES} ${CURL_LIBRARIES} regex)

# Ensure the executable uses the GTK framework correctly
target_compile_options(nuloan_project PRIVATE ${GTK4_CFLAGS})

# Optional: Add additional settings for linking, if needed
set_target_properties(nuloan_project PROPERTIES LINK_FLAGS "-Wl,--enable-auto-import")
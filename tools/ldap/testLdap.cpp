#include <windows.h>  
#include <winldap.h>
#include <stdio.h> 

#pragma comment(lib,"crypt32.lib")
#pragma comment(lib,"Wldap32.lib")



int testLdap() {
	int ret = -1;
	char *ip = "172.16.226.173";
	int port = 389;
	char *userDn = "cn=Manager,dc=test,dc=com"; char *passwd = "secret";
	//char* userDn = "uid=user1,ou=people,dc=test,dc=com"; char *passwd = "123";

	
	
	int version;
	LDAP* ld;

	ld = ldap_open(ip, port);
	if (ld == NULL) {
		printf("connect failed\n");
		return -1;
	}
	printf("connect success\n");
	version = LDAP_VERSION3;
	if (ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &version) < 0)
	{
		perror("ldap_set_option LDAPv3");
		goto err;
	}

	//1. The session handle.
	//2. The name of the user to bind as. The bind operation uses the dn and passwd parameters to authenticate the user.
	//3. The password of the user specified in the dn parameter.
	if ((ret = ldap_simple_bind_s(ld, userDn, passwd)) != LDAP_SUCCESS) {
		fprintf(stderr, "ldap_simple_bind_s: %s\n", ldap_err2string(ret));
		goto err;
	}
	printf("bind success\n");

	// search
	{
		LDAPMessage *res = 0;
		char* baseDn = "ou=People1,dc=test,dc=com";
		/*
			ld		Session handle.
			base	Pointer to a null-terminated string that contains the distinguished name of the entry at which to start the search.
			scope	Specifies one of the following values to indicate the search scope.
					Value Meaning
						LDAP_SCOPE_BASE Search the base-entry only.
						LDAP_SCOPE_ONELEVEL Search all entries in the first level below the base-entry, excluding the base-entry.
						LDAP_SCOPE_SUBTREE Search the base-entry and all entries in the tree below the base.
			filter	Pointer to a null-terminated string that specifies the search filter. For more information, see Search Filter Syntax.

			attrs		A null-terminated array of null-terminated strings indicating the attributes to return for each matching entry. Pass NULL to retrieve all available attributes.
			attrsonly	Boolean value that should be zero if both attribute types and values are to be returned, nonzero if only types are required.
			res			Contains the results of the search upon completion of the call. Can also contain partial results or extended data when the function call fails with an error code. Free results returned with a call to ldap_msgfree when they are no longer required by the application.
		*/
		// "(objectClass=*)"
		// "(&(objectCategory=person)(objectClass=user)(!cn=andy))"
		// "(sn=sm*)"
		// "(&(objectCategory=person)(objectClass=contact)(|(sn=Smith)(sn=Johnson)))"
		char *filter = "(objectClass=*)";
		ret = ldap_search_s(ld, baseDn, LDAP_SCOPE_SUBTREE, filter, NULL, 0, &res);
		if (ret != LDAP_SUCCESS)
		{
			fprintf(stderr, "ldap_search_s: %s\n", ldap_err2string(ret));
			goto err;
		}

		for (LDAPMessage* e = ldap_first_entry(ld, res);e != NULL;e = ldap_next_entry(ld, e))
		{
			BerElement* ptr;
			PCHAR dn = ldap_get_dn(ld, e);
			printf("dn: %s \n", dn);
			ldap_memfree(dn);
			for (PCHAR a = ldap_first_attribute(ld, e, &ptr);a != NULL;a = ldap_next_attribute(ld, e, ptr))
			{
				//printf( "attr= %s:  ",a );
				PCHAR* vals = ldap_get_values(ld, e, a);
				for (int i = 0; vals[i] != NULL; i++)
					printf("\t%s=%s \n", a, vals[i]);

				ldap_value_free(vals);
			}
			printf("\n");


		}
		ldap_msgfree(res);
	}

	//add
	{
		char *cn_values[] = { "ea",NULL };
		char *sn_values[] = { "ea",NULL };
		char *userPassword_values[] = { "123456",NULL };
		char *objectClass_values[] = { "person",NULL };
		
		char* baseDn = "cn=ea,ou=People1,dc=test,dc=com";

		/* 
			mod_op 
				Specifies one of the following values to indicate the modification operation to perform. 
					LDAP_MOD_ADD (0x00)	Adds a value to the entry.The supplied values are appended to the existing values in the attribute.
			mod_type
				Pointer to a null-terminated string that specifies the name of the attribute to modify.
			mod_vals
				Pointer to an array of values
		*/
		LDAPMod mod0 = { LDAP_MOD_ADD,"cn",cn_values };
		LDAPMod mod1 = { LDAP_MOD_ADD,"sn",sn_values };
		LDAPMod mod2 = { LDAP_MOD_ADD,"objectClass",objectClass_values };
		LDAPMod mod3 = { LDAP_MOD_ADD,"userPassword",userPassword_values };
		LDAPMod *lmod[] = { &mod0,&mod1,&mod2,&mod3,NULL };

		// ld The session handle.
		// dn A pointer to a null-terminated string that contains the distinguished name of the entry to add.
		// attrs An array of pointers to LDAPMod structures.Each structure specifies a single attribute.See Remarks for more information.
		ret = ldap_add_s(ld, baseDn, lmod);
		if (ret != LDAP_SUCCESS)
		{
			fprintf(stderr, "ldap_add_s: %s\n", ldap_err2string(ret));
			//goto err;
		}
		else
		{
			printf("add success\n");
		}
		
	}

	// modify
	{
		char *passwd[] = { "a654321",NULL };
		LDAPMod mod1 = { LDAP_MOD_REPLACE,"userPassword",passwd };
		LDAPMod *lmod[] = { &mod1,NULL };
		char* baseDn = "cn=ea,ou=People1,dc=test,dc=com";
		ret = ldap_modify_s(ld, baseDn, lmod);
		if (ret != LDAP_SUCCESS)
		{
			fprintf(stderr, "ldap_modify_s: %s\n", ldap_err2string(ret));
			//goto err;
		}
		else
		{
			printf("modify success\n");
		}
		
	}
	//del
	{
		char* baseDn = "cn=ea,ou=People1,dc=test,dc=com";
		ret = ldap_delete_s(ld, baseDn);
		if (ret != LDAP_SUCCESS)
		{
			fprintf(stderr, "ldap_delete_s: %s\n", ldap_err2string(ret));
			//goto err;
		}
		else
		{
			printf("delete success\n");
		}
	}
	ret = 0;
err:
	ldap_unbind_s(ld);
	return ret;

}
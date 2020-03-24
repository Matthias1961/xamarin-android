#ifndef INC_MONODROID_DESIGNER_ASSEMBLIES_H
#define INC_MONODROID_DESIGNER_ASSEMBLIES_H

#include "jni-wrappers.hh"
#include <string.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/assembly.h>

#define DEFAULT_CAPACITY 8
#define DEFAULT_CONTEXT_ID -1

namespace xamarin::android::internal {
	class DesignerAssemblies
	{
		private:
			struct DesignerAssemblyEntry
			{
				int context_id;
				MonoObject *alc;
				unsigned int assemblies_count;
				char **names;
				char **assemblies_bytes;
				char **assemblies_paths;
				unsigned int *assemblies_bytes_len;

				DesignerAssemblyEntry (MonoDomain *domain, JNIEnv *env, jstring_array_wrapper &assemblies, jobjectArray assembliesBytes, jstring_array_wrapper &assembliesPaths);
				~DesignerAssemblyEntry ();
			};

		public:
			DesignerAssemblies ()
			{
				capacity = DEFAULT_CAPACITY;
				entries = new DesignerAssemblyEntry*[DEFAULT_CAPACITY];
			}

			bool has_assemblies () const { return length > 0; }
			MonoAssembly* try_load_assembly (MonoDomain *domain, int context_id, MonoAssemblyName *name);
			int add_or_update_from_java (MonoDomain *domain, JNIEnv *env, jstring_array_wrapper &assemblies, jobjectArray assembliesBytes, jstring_array_wrapper &assembliesPaths);
			void clear_for_context (MonoDomain *domain, int context_id);
			MonoObject* get_alc_for_context (MonoDomain *domain, int context_id);

		private:
			DesignerAssemblyEntry **entries;
			unsigned int capacity;
			unsigned int length;

			DesignerAssemblyEntry* find_entry (int domain_id);
			void add_or_replace_entry (DesignerAssemblyEntry *new_entry);
			void add_entry (DesignerAssemblyEntry *entry);
			DesignerAssemblyEntry* remove_entry (int domain_id);
	};
}

#endif
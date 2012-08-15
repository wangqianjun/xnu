 * Copyright (c) 2004 Apple Computer, Inc. All rights reserved.
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.


#include <bsm/audit.h>
#include <bsm/audit_record.h>
#include <bsm/audit_kernel.h>
#include <bsm/audit_kevents.h>
#include <bsm/audit_klib.h>

	au_evclassmap_init();
		rec = (struct au_record *)kalloc(sizeof(*rec));
		rec->data = (u_char *)kalloc(MAX_AUDIT_RECORD_SIZE * sizeof(u_char));
			kfree((vm_offset_t)rec, (vm_size_t)sizeof(*rec));
		kfree((vm_offset_t)tok, sizeof(*tok) + tok->len);
#define UPATH1_KPATH1_VNODE1_TOKENS	\
		if (ar->ar_valid_arg & ARG_UPATH1) {  		\
			tok = au_to_path(ar->ar_arg_upath1);	\
			kau_write(rec, tok);			\
		}						\
			tok = kau_to_attr32(&ar->ar_arg_vnode1);\
 
#define KPATH1_VNODE1_TOKENS	\
			tok = kau_to_attr32(&ar->ar_arg_vnode1);\
			tok = kau_to_attr32(&ar->ar_arg_vnode1);\
				tok = kau_to_attr32(&ar->ar_arg_vnode1);\
#define PROCESS_PID_TOKENS(argn)	\
	do { \
		if ((ar->ar_arg_pid > 0) /* Kill a single process */	\
		    && (ar->ar_valid_arg & ARG_PROCESS)) {		\
			tok = au_to_process(ar->ar_arg_auid, ar->ar_arg_euid, \
				ar->ar_arg_egid, ar->ar_arg_ruid,	\
				ar->ar_arg_rgid, ar->ar_arg_pid,	\
				ar->ar_arg_asid, &ar->ar_arg_termid);	\
			kau_write(rec, tok);				\
		} else {						\
			tok = au_to_arg32(argn, "process", ar->ar_arg_pid);\
			kau_write(rec, tok);				\
		}							\
	} while (0)							\

/*
 * Implement auditing for the auditon() system call. The audit tokens
 * that are generated depend on the command that was sent into the 
 * auditon() system call.
 *
 */
void
audit_sys_auditon(struct audit_record *ar, struct au_record *rec)
{
	struct au_token *tok;

	switch (ar->ar_arg_cmd) {
        case A_SETPOLICY:
		if (sizeof(ar->ar_arg_auditon.au_flags) > 4)
			tok = au_to_arg64(1, "policy", 
				ar->ar_arg_auditon.au_flags);
		else
			tok = au_to_arg32(1, "policy", 
				ar->ar_arg_auditon.au_flags);
		kau_write(rec, tok);
		break;
        case A_SETKMASK:
		tok = au_to_arg32(2, "setkmask:as_success", 
			ar->ar_arg_auditon.au_mask.am_success);
		kau_write(rec, tok);
		tok = au_to_arg32(2, "setkmask:as_failure", 
			ar->ar_arg_auditon.au_mask.am_failure);
		kau_write(rec, tok);
		break;
        case A_SETQCTRL:
		tok = au_to_arg32(3, "setqctrl:aq_hiwater", 
			ar->ar_arg_auditon.au_qctrl.aq_hiwater);
		kau_write(rec, tok);
		tok = au_to_arg32(3, "setqctrl:aq_lowater", 
			ar->ar_arg_auditon.au_qctrl.aq_lowater);
		kau_write(rec, tok);
		tok = au_to_arg32(3, "setqctrl:aq_bufsz", 
			ar->ar_arg_auditon.au_qctrl.aq_bufsz);
		kau_write(rec, tok);
		tok = au_to_arg32(3, "setqctrl:aq_delay", 
			ar->ar_arg_auditon.au_qctrl.aq_delay);
		kau_write(rec, tok);
		tok = au_to_arg32(3, "setqctrl:aq_minfree", 
			ar->ar_arg_auditon.au_qctrl.aq_minfree);
		kau_write(rec, tok);
		break;
        case A_SETUMASK:
		tok = au_to_arg32(3, "setumask:as_success", 
			ar->ar_arg_auditon.au_auinfo.ai_mask.am_success);
		kau_write(rec, tok);
		tok = au_to_arg32(3, "setumask:as_failure", 
			ar->ar_arg_auditon.au_auinfo.ai_mask.am_failure);
		kau_write(rec, tok);
		break;
        case A_SETSMASK:
		tok = au_to_arg32(3, "setsmask:as_success", 
			ar->ar_arg_auditon.au_auinfo.ai_mask.am_success);
		kau_write(rec, tok);
		tok = au_to_arg32(3, "setsmask:as_failure", 
			ar->ar_arg_auditon.au_auinfo.ai_mask.am_failure);
		kau_write(rec, tok);
		break;
        case A_SETCOND:
		if (sizeof(ar->ar_arg_auditon.au_cond) > 4)
			tok = au_to_arg64(3, "setcond", 
				ar->ar_arg_auditon.au_cond);
		else
			tok = au_to_arg32(3, "setcond", 
				ar->ar_arg_auditon.au_cond);
		kau_write(rec, tok);
		break;
        case A_SETCLASS:
		tok = au_to_arg32(2, "setclass:ec_event",
			ar->ar_arg_auditon.au_evclass.ec_number);
		kau_write(rec, tok);
		tok = au_to_arg32(3, "setclass:ec_class",
			ar->ar_arg_auditon.au_evclass.ec_class);
		kau_write(rec, tok);
		break;
        case A_SETPMASK:
		tok = au_to_arg32(2, "setpmask:as_success", 
			ar->ar_arg_auditon.au_aupinfo.ap_mask.am_success);
		kau_write(rec, tok);
		tok = au_to_arg32(2, "setpmask:as_failure", 
			ar->ar_arg_auditon.au_aupinfo.ap_mask.am_failure);
		kau_write(rec, tok);
		break;
        case A_SETFSIZE:
		tok = au_to_arg32(2, "setfsize:filesize", 
			ar->ar_arg_auditon.au_fstat.af_filesz);
		kau_write(rec, tok);
		break;
	default:
		break;
	}
}

		tok = au_to_arg32(1,"domain", ar->ar_arg_sockinfo.so_domain);
		tok = au_to_arg32(2,"type", ar->ar_arg_sockinfo.so_type);
		tok = au_to_arg32(3,"protocol",ar->ar_arg_sockinfo.so_protocol);
	case AUE_ACCT:
		if (ar->ar_valid_arg & (ARG_KPATH1 | ARG_UPATH1)) {
			UPATH1_KPATH1_VNODE1_TOKENS;
		} else {
			tok = au_to_arg32(1, "accounting off", 0);
			kau_write(rec, tok);
		}
		break;

		break;

	case AUE_SETAUDIT:
		if (ar->ar_valid_arg & ARG_AUID) {
			tok = au_to_arg32(1, "setaudit:auid", ar->ar_arg_auid);
			kau_write(rec, tok);
			tok = au_to_arg32(1, "setaudit:port", 
					ar->ar_arg_termid.port);
			kau_write(rec, tok);
			tok = au_to_arg32(1, "setaudit:machine", 
					ar->ar_arg_termid.machine);
			kau_write(rec, tok);
			tok = au_to_arg32(1, "setaudit:as_success", 
					ar->ar_arg_amask.am_success);
			kau_write(rec, tok);
			tok = au_to_arg32(1, "setaudit:as_failure", 
					ar->ar_arg_amask.am_failure);
			kau_write(rec, tok);
			tok = au_to_arg32(1, "setaudit:asid", ar->ar_arg_asid);
			kau_write(rec, tok);
		}
		break;

	case AUE_SETAUDIT_ADDR:
		break;		/* XXX need to add arguments */

	case AUE_AUDITON:
		/* For AUDITON commands without own event, audit the cmd */
		tok = au_to_arg32(1, "cmd", ar->ar_arg_cmd);
		kau_write(rec, tok);
		/* fall thru */

	case AUE_AUDITON_GETCAR:
	case AUE_AUDITON_GETCLASS:
	case AUE_AUDITON_GETCOND:
	case AUE_AUDITON_GETCWD:
	case AUE_AUDITON_GETKMASK:
	case AUE_AUDITON_GETSTAT:
	case AUE_AUDITON_GPOLICY:
	case AUE_AUDITON_GQCTRL:
	case AUE_AUDITON_SETCLASS:
	case AUE_AUDITON_SETCOND:
	case AUE_AUDITON_SETKMASK:
	case AUE_AUDITON_SETSMASK:
	case AUE_AUDITON_SETSTAT:
	case AUE_AUDITON_SETUMASK:
	case AUE_AUDITON_SPOLICY:
	case AUE_AUDITON_SQCTRL:
		if (ar->ar_valid_arg & ARG_AUDITON) {
			audit_sys_auditon(ar, rec);
		}
		break;
	
	case AUE_AUDITCTL:
		UPATH1_KPATH1_VNODE1_TOKENS;
		break;

	case AUE_GETAUDIT:
	case AUE_GETAUDIT_ADDR:
	case AUE_SETSID:
	case AUE_SETTIMEOFDAY:
	case AUE_NEWSYSTEMSHREG:
		UPATH1_KPATH1_VNODE1_TOKENS;
		UPATH1_KPATH1_VNODE1_TOKENS;
		UPATH1_KPATH1_VNODE1_TOKENS;
		UPATH1_KPATH1_VNODE1_TOKENS;
		UPATH1_KPATH1_VNODE1_TOKENS;
		UPATH1_KPATH1_VNODE1_TOKENS;

	case AUE_NFSSVC:
		tok = au_to_arg32(1, "request", ar->ar_arg_cmd);
		kau_write(rec, tok);
		if (ar->ar_valid_arg & (ARG_KPATH1 | ARG_UPATH1)) {
			UPATH1_KPATH1_VNODE1_TOKENS;
		}
		break;

		tok = au_to_arg32(2, "cmd", ar->ar_arg_cmd);
		kau_write(rec, tok);
		if (ar->ar_valid_arg & ARG_VNODE1) {
	case AUE_FORK:
	case AUE_VFORK:
		tok = au_to_arg32(0, "child PID", ar->ar_arg_pid);
		kau_write(rec, tok);
		break;
	
	case AUE_IOCTL:
		tok = au_to_arg32(2, "cmd", ar->ar_arg_cmd);
		kau_write(rec, tok);
		tok = au_to_arg32(1, "arg", (u_int32_t)ar->ar_arg_addr);
		kau_write(rec, tok);
		if (ar->ar_valid_arg & ARG_VNODE1) {
			FD_KPATH1_VNODE1_TOKENS;
		} else {
			if (ar->ar_valid_arg & ARG_SOCKINFO) {
			    tok = kau_to_socket(&ar->ar_arg_sockinfo); 
			    kau_write(rec, tok);
			} else {
			    tok = au_to_arg32(1, "fd", ar->ar_arg_fd);
			    kau_write(rec, tok);
			}
		}
		break;

	case AUE_KILL:
		tok = au_to_arg32(2, "signal", ar->ar_arg_signum);
		kau_write(rec, tok);
		PROCESS_PID_TOKENS(1);
		break;

	case AUE_KTRACE:
		tok = au_to_arg32(2, "ops", ar->ar_arg_cmd);
		kau_write(rec, tok);
		tok = au_to_arg32(3, "trpoints", ar->ar_arg_value);
		kau_write(rec, tok);
		PROCESS_PID_TOKENS(4);
		UPATH1_KPATH1_VNODE1_TOKENS;
		break;

		UPATH1_KPATH1_VNODE1_TOKENS;
	case AUE_LOADSHFILE:
		tok = au_to_arg32(4, "base addr", (u_int32_t)ar->ar_arg_addr);
		kau_write(rec, tok);
		UPATH1_KPATH1_VNODE1_TOKENS;
		break;
	
		UPATH1_KPATH1_VNODE1_TOKENS;
		UPATH1_KPATH1_VNODE1_TOKENS;
		break;

	case AUE_MMAP:
	case AUE_MUNMAP:
	case AUE_MPROTECT:
	case AUE_MLOCK:
	case AUE_MUNLOCK:
	case AUE_MINHERIT:
		tok = au_to_arg32(1, "addr", (u_int32_t)ar->ar_arg_addr);
		kau_write(rec, tok);
		tok = au_to_arg32(2, "len", ar->ar_arg_len);
		kau_write(rec, tok);
		if (ar->ar_event == AUE_MMAP)
			FD_KPATH1_VNODE1_TOKENS;
		if (ar->ar_event == AUE_MPROTECT) {
			tok = au_to_arg32(3, "protection", ar->ar_arg_value);
			kau_write(rec, tok);
		}
		if (ar->ar_event == AUE_MINHERIT) {
			tok = au_to_arg32(3, "inherit", ar->ar_arg_value);
			kau_write(rec, tok);
		}
		if (ar->ar_valid_arg & ARG_TEXT) {
	case AUE_UNMOUNT:
		UPATH1_KPATH1_VNODE1_TOKENS;
	case AUE_RESETSHFILE:
		tok = au_to_arg32(1, "base addr", (u_int32_t)ar->ar_arg_addr);
		kau_write(rec, tok);
		break;
	
		tok = au_to_arg32(3, "mode", ar->ar_arg_mode);
		kau_write(rec, tok);
		/* fall thru */

	case AUE_OPEN:
	case AUE_OPEN_R:
	case AUE_OPEN_RT:
	case AUE_OPEN_RW:
	case AUE_OPEN_RWT:
	case AUE_OPEN_W:
		tok = au_to_arg32(2, "flags", ar->ar_arg_fflags);
		kau_write(rec, tok);
		UPATH1_KPATH1_VNODE1_TOKENS;
		break;

	case AUE_PTRACE:
		tok = au_to_arg32(1, "request", ar->ar_arg_cmd);
		kau_write(rec, tok);
		tok = au_to_arg32(3, "addr", (u_int32_t)ar->ar_arg_addr);
		kau_write(rec, tok);
		tok = au_to_arg32(4, "data", ar->ar_arg_value);
		kau_write(rec, tok);
		PROCESS_PID_TOKENS(2);
		UPATH1_KPATH1_VNODE1_TOKENS;
		break;

	case AUE_REBOOT:
		tok = au_to_arg32(1, "howto", ar->ar_arg_cmd);
		kau_write(rec, tok);

	case AUE_SETLOGIN:
		if (ar->ar_valid_arg & ARG_TEXT) {
			tok = au_to_text(ar->ar_arg_text);
			kau_write(rec, tok);
		}
		break;

	case AUE_SETPRIORITY:
		tok = au_to_arg32(1, "which", ar->ar_arg_cmd);
		kau_write(rec, tok);
		tok = au_to_arg32(2, "who", ar->ar_arg_uid);
		kau_write(rec, tok);
		tok = au_to_arg32(2, "priority", ar->ar_arg_value);
		kau_write(rec, tok);
		break;

	case AUE_SETPRIVEXEC:
		tok = au_to_arg32(1, "flag", ar->ar_arg_value);
		kau_write(rec, tok);
		break;

	/* AUE_SHMAT, AUE_SHMCTL, AUE_SHMDT and AUE_SHMGET are SysV IPC */
	/* AUE_SHMOPEN, AUE_SHMUNLINK, AUE_SEMOPEN, AUE_SEMCLOSE 
	 * and AUE_SEMUNLINK are Posix IPC */
	case AUE_SHMOPEN:
		tok = au_to_arg32(2, "flags", ar->ar_arg_fflags);
		kau_write(rec, tok);
		tok = au_to_arg32(3, "mode", ar->ar_arg_mode);
		kau_write(rec, tok);
	case AUE_SHMUNLINK:
		if (ar->ar_valid_arg & ARG_TEXT) {
			tok = au_to_text(ar->ar_arg_text);
			kau_write(rec, tok);
		}
		if (ar->ar_valid_arg & ARG_POSIX_IPC_PERM) {
		/* Create an ipc_perm token */
			struct ipc_perm perm;
			perm.uid = ar->ar_arg_pipc_perm.pipc_uid;
			perm.gid = ar->ar_arg_pipc_perm.pipc_gid;
			perm.cuid = ar->ar_arg_pipc_perm.pipc_uid;
			perm.cgid = ar->ar_arg_pipc_perm.pipc_gid;
			perm.mode = ar->ar_arg_pipc_perm.pipc_mode;
			perm.seq = 0;
			perm.key = 0;
			tok = au_to_ipc_perm(&perm);
			kau_write(rec, tok);
		}
		break;

	case AUE_SEMOPEN:
		tok = au_to_arg32(2, "flags", ar->ar_arg_fflags);
		kau_write(rec, tok);
		tok = au_to_arg32(3, "mode", ar->ar_arg_mode);
		kau_write(rec, tok);
		tok = au_to_arg32(4, "value", ar->ar_arg_value);
		kau_write(rec, tok);
		/* fall through */
	case AUE_SEMUNLINK:
		if (ar->ar_valid_arg & ARG_TEXT) {
			tok = au_to_text(ar->ar_arg_text);
			kau_write(rec, tok);
		}
		if (ar->ar_valid_arg & ARG_POSIX_IPC_PERM) {
		/* Create an ipc_perm token */
			struct ipc_perm perm;
			perm.uid = ar->ar_arg_pipc_perm.pipc_uid;
			perm.gid = ar->ar_arg_pipc_perm.pipc_gid;
			perm.cuid = ar->ar_arg_pipc_perm.pipc_uid;
			perm.cgid = ar->ar_arg_pipc_perm.pipc_gid;
			perm.mode = ar->ar_arg_pipc_perm.pipc_mode;
			perm.seq = 0;
			perm.key = 0;
			tok = au_to_ipc_perm(&perm);
			kau_write(rec, tok);
		}
		break;

	case AUE_SEMCLOSE:
		tok = au_to_arg32(1, "sem", ar->ar_arg_fd);
		kau_write(rec, tok);
		break;

		UPATH1_KPATH1_VNODE1_TOKENS;
		break;

	case AUE_SYSCTL:
	case AUE_SYSCTL_NONADMIN:
		if (ar->ar_valid_arg & (ARG_CTLNAME | ARG_LEN)) {
			for (ctr = 0; ctr < ar->ar_arg_len; ctr++) {
			  tok = au_to_arg32(1, "name", ar->ar_arg_ctlname[ctr]);
			  kau_write(rec, tok);
			}
		}
		if (ar->ar_valid_arg & ARG_VALUE) {
			tok = au_to_arg32(5, "newval", ar->ar_arg_value);
			kau_write(rec, tok);
		}
		if (ar->ar_valid_arg & ARG_TEXT) {
			tok = au_to_text(ar->ar_arg_text);
			kau_write(rec, tok);
		}
	/************************
	 * Mach system calls    *
	 ************************/
	case AUE_INITPROCESS:
		break;

	case AUE_PIDFORTASK:
		tok = au_to_arg32(1, "port", (u_int32_t)ar->ar_arg_mach_port1);
		kau_write(rec, tok);
		if (ar->ar_valid_arg & ARG_PID) {
			tok = au_to_arg32(2, "pid", (u_int32_t)ar->ar_arg_pid);
			kau_write(rec, tok);
		}
		break;

	case AUE_TASKFORPID:
		tok = au_to_arg32(1, "target port", 
			(u_int32_t)ar->ar_arg_mach_port1);
		kau_write(rec, tok);
		if (ar->ar_valid_arg & ARG_MACHPORT2) {
			tok = au_to_arg32(3, "task port", 
				(u_int32_t)ar->ar_arg_mach_port2);
			kau_write(rec, tok);
		}
		PROCESS_PID_TOKENS(2);
		break;

	case AUE_SWAPON:
		tok = au_to_arg32(4, "priority", 
			(u_int32_t)ar->ar_arg_value);
		kau_write(rec, tok);
		UPATH1_KPATH1_VNODE1_TOKENS;
		break;

	case AUE_SWAPOFF:
		UPATH1_KPATH1_VNODE1_TOKENS;
		break;

	case AUE_MAPFD:
		tok = au_to_arg32(3, "va", (u_int32_t)ar->ar_arg_addr);
		kau_write(rec, tok);
		FD_KPATH1_VNODE1_TOKENS;
		break;

bsm_rec_verify(void *rec)
	char c = *(char *)rec;
	if ( (c != AU_HEADER_32_TOKEN) && 
		(c != AU_HEADER_EX_32_TOKEN) && 
		(c != AU_HEADER_64_TOKEN) && 
		(c != AU_HEADER_EX_64_TOKEN) ) {
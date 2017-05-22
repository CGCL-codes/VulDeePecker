/*
 * Copyright (C) 2000, 2003, 2004, 2005, 2007, 2008, 2009, 2010 Free
 * Software Foundation, Inc.
 *
 * Author: Nikos Mavrogiannopoulos
 *
 * This file is part of GnuTLS.
 *
 * The GnuTLS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
 * USA
 *
 */
#include "gnutls_int.h"
#include "gnutls_errors.h"
#include "debug.h"
#include <gnutls_session_pack.h>
#include <gnutls_datum.h>

/**
 * gnutls_session_get_data:
 * @session: is a #gnutls_session_t structure.
 * @session_data: is a pointer to space to hold the session.
 * @session_data_size: is the session_data's size, or it will be set by the function.
 *
 * Returns all session parameters, in order to support resuming.  The
 * client should call this, and keep the returned session, if he
 * wants to resume that current version later by calling
 * gnutls_session_set_data() This function must be called after a
 * successful handshake.
 *
 * Resuming sessions is really useful and speedups connections after
 * a successful one.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise
 *   an error code is returned.
 **/
int
gnutls_session_get_data (gnutls_session_t session,
                         void *session_data, size_t * session_data_size)
{

  gnutls_datum_t psession;
  int ret;

  if (session->internals.resumable == RESUME_FALSE)
    return GNUTLS_E_INVALID_SESSION;

  psession.data = session_data;

  ret = _gnutls_session_pack (session, &psession);
  if (ret < 0)
    {
      gnutls_assert ();
      return ret;
    }
  *session_data_size = psession.size;

  if (psession.size > *session_data_size)
    {
      ret = GNUTLS_E_SHORT_MEMORY_BUFFER;
      goto error;
    }

  if (session_data != NULL)
    memcpy (session_data, psession.data, psession.size);

  ret = 0;

error:
  _gnutls_free_datum (&psession);
  return ret;
}

/**
 * gnutls_session_get_data2:
 * @session: is a #gnutls_session_t structure.
 * @data: is a pointer to a datum that will hold the session.
 *
 * Returns all session parameters, in order to support resuming.  The
 * client should call this, and keep the returned session, if he wants
 * to resume that current version later by calling
 * gnutls_session_set_data().  This function must be called after a
 * successful handshake.  The returned datum must be freed with
 * gnutls_free().
 *
 * Resuming sessions is really useful and speedups connections after
 * a successful one.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise
 *   an error code is returned.
 **/
int
gnutls_session_get_data2 (gnutls_session_t session, gnutls_datum_t * data)
{

  int ret;

  if (data == NULL)
    {
      return GNUTLS_E_INVALID_REQUEST;
    }

  if (session->internals.resumable == RESUME_FALSE)
    return GNUTLS_E_INVALID_SESSION;

  ret = _gnutls_session_pack (session, data);
  if (ret < 0)
    {
      gnutls_assert ();
      return ret;
    }

  return 0;
}


/**
 * gnutls_session_get_id:
 * @session: is a #gnutls_session_t structure.
 * @session_id: is a pointer to space to hold the session id.
 * @session_id_size: is the session id's size, or it will be set by the function.
 *
 * Returns the current session id. This can be used if you want to
 * check if the next session you tried to resume was actually
 * resumed.  This is because resumed sessions have the same sessionID
 * with the original session.
 *
 * Session id is some data set by the server, that identify the
 * current session.  In TLS 1.0 and SSL 3.0 session id is always less
 * than 32 bytes.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise
 *   an error code is returned.
 **/
int
gnutls_session_get_id (gnutls_session_t session,
                       void *session_id, size_t * session_id_size)
{
  size_t given_session_id_size = *session_id_size;

  *session_id_size = session->security_parameters.session_id_size;

  /* just return the session size */
  if (session_id == NULL)
    {
      return 0;
    }

  if (given_session_id_size < session->security_parameters.session_id_size)
    {
      return GNUTLS_E_SHORT_MEMORY_BUFFER;
    }

  memcpy (session_id, &session->security_parameters.session_id,
          *session_id_size);

  return 0;
}

/**
 * gnutls_session_set_data:
 * @session: is a #gnutls_session_t structure.
 * @session_data: is a pointer to space to hold the session.
 * @session_data_size: is the session's size
 *
 * Sets all session parameters, in order to resume a previously
 * established session.  The session data given must be the one
 * returned by gnutls_session_get_data().  This function should be
 * called before gnutls_handshake().
 *
 * Keep in mind that session resuming is advisory. The server may
 * choose not to resume the session, thus a full handshake will be
 * performed.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise
 *   an error code is returned.
 **/
int
gnutls_session_set_data (gnutls_session_t session,
                         const void *session_data, size_t session_data_size)
{
  int ret;
  gnutls_datum_t psession;

  psession.data = (opaque *) session_data;
  psession.size = session_data_size;

  if (session_data == NULL || session_data_size == 0)
    {
      gnutls_assert ();
      return GNUTLS_E_INVALID_REQUEST;
    }
  ret = _gnutls_session_unpack (session, &psession);
  if (ret < 0)
    {
      gnutls_assert ();
      return ret;
    }

  return 0;
}

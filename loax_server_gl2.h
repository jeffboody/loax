/*
 * Copyright (c) 2013 Jeff Boody
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef loax_server_gl2_H
#define loax_server_gl2_H

void loaxSwapBuffers (void);
void loaxSize (void);
void loaxActiveTexture (void);
void loaxAttachShader (void);
void loaxBindAttribLocation (void);
void loaxBindBuffer (void);
void loaxBindFramebuffer (void);
void loaxBindRenderbuffer (void);
void loaxBindTexture (void);
void loaxBlendColor (void);
void loaxBlendEquation (void);
void loaxBlendEquationSeparate (void);
void loaxBlendFunc (void);
void loaxBlendFuncSeparate (void);
void loaxBufferData (void);
void loaxBufferSubData (void);
void loaxCheckFramebufferStatus (void);
void loaxClear (void);
void loaxClearColor (void);
void loaxClearDepthf (void);
void loaxClearStencil (void);
void loaxColorMask (void);
void loaxCompileShader (void);
void loaxCompressedTexImage2D (void);
void loaxCompressedTexSubImage2D (void);
void loaxCopyTexImage2D (void);
void loaxCopyTexSubImage2D (void);
void loaxCreateProgram (void);
void loaxCreateShader (void);
void loaxCullFace (void);
void loaxDeleteBuffers (void);
void loaxDeleteFramebuffers (void);
void loaxDeleteProgram (void);
void loaxDeleteRenderbuffers (void);
void loaxDeleteShader (void);
void loaxDeleteTextures (void);
void loaxDepthFunc (void);
void loaxDepthMask (void);
void loaxDepthRangef (void);
void loaxDetachShader (void);
void loaxDisable (void);
void loaxDisableVertexAttribArray (void);
void loaxDrawArrays (void);
void loaxDrawElements (void);
void loaxEnable (void);
void loaxEnableVertexAttribArray (void);
void loaxFinish (void);
void loaxFlush (void);
void loaxFramebufferRenderbuffer (void);
void loaxFramebufferTexture2D (void);
void loaxFrontFace (void);
void loaxGenBuffers (void);
void loaxGenerateMipmap (void);
void loaxGenFramebuffers (void);
void loaxGenRenderbuffers (void);
void loaxGenTextures (void);
void loaxGetActiveAttrib (void);
void loaxGetActiveUniform (void);
void loaxGetAttachedShaders (void);
void loaxGetAttribLocation (void);
void loaxGetBooleanv (void);
void loaxGetBufferParameteriv (void);
void loaxGetError (void);
void loaxGetFloatv (void);
void loaxGetFramebufferAttachmentParameteriv (void);
void loaxGetIntegerv (void);
void loaxGetProgramiv (void);
void loaxGetProgramInfoLog (void);
void loaxGetRenderbufferParameteriv (void);
void loaxGetShaderiv (void);
void loaxGetShaderInfoLog (void);
void loaxGetShaderPrecisionFormat (void);
void loaxGetShaderSource (void);
void loaxGetString (void);
void loaxGetTexParameterfv (void);
void loaxGetTexParameteriv (void);
void loaxGetUniformfv (void);
void loaxGetUniformiv (void);
void loaxGetUniformLocation (void);
void loaxGetVertexAttribfv (void);
void loaxGetVertexAttribiv (void);
void loaxGetVertexAttribPointerv (void);
void loaxHint (void);
void loaxIsBuffer (void);
void loaxIsEnabled (void);
void loaxIsFramebuffer (void);
void loaxIsProgram (void);
void loaxIsRenderbuffer (void);
void loaxIsShader (void);
void loaxIsTexture (void);
void loaxLineWidth (void);
void loaxLinkProgram (void);
void loaxPixelStorei (void);
void loaxPolygonOffset (void);
void loaxReadPixels (void);
void loaxReleaseShaderCompiler (void);
void loaxRenderbufferStorage (void);
void loaxSampleCoverage (void);
void loaxScissor (void);
void loaxShaderBinary (void);
void loaxShaderSource (void);
void loaxStencilFunc (void);
void loaxStencilFuncSeparate (void);
void loaxStencilMask (void);
void loaxStencilMaskSeparate (void);
void loaxStencilOp (void);
void loaxStencilOpSeparate (void);
void loaxTexImage2D (void);
void loaxTexParameterf (void);
void loaxTexParameterfv (void);
void loaxTexParameteri (void);
void loaxTexParameteriv (void);
void loaxTexSubImage2D (void);
void loaxUniform1f (void);
void loaxUniform1fv (void);
void loaxUniform1i (void);
void loaxUniform1iv (void);
void loaxUniform2f (void);
void loaxUniform2fv (void);
void loaxUniform2i (void);
void loaxUniform2iv (void);
void loaxUniform3f (void);
void loaxUniform3fv (void);
void loaxUniform3i (void);
void loaxUniform3iv (void);
void loaxUniform4f (void);
void loaxUniform4fv (void);
void loaxUniform4i (void);
void loaxUniform4iv (void);
void loaxUniformMatrix2fv (void);
void loaxUniformMatrix3fv (void);
void loaxUniformMatrix4fv (void);
void loaxUseProgram (void);
void loaxValidateProgram (void);
void loaxVertexAttrib1f (void);
void loaxVertexAttrib1fv (void);
void loaxVertexAttrib2f (void);
void loaxVertexAttrib2fv (void);
void loaxVertexAttrib3f (void);
void loaxVertexAttrib3fv (void);
void loaxVertexAttrib4f (void);
void loaxVertexAttrib4fv (void);
void loaxVertexAttribPointer (void);
void loaxViewport (void);

#endif

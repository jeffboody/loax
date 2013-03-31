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

#ifndef loax_gl2_H
#define loax_gl2_H

#define LOAX_SwapBuffers                         0
#define LOAX_Size                                1
#define LOAX_ActiveTexture                       2
#define LOAX_AttachShader                        3
#define LOAX_BindAttribLocation                  4
#define LOAX_BindBuffer                          5
#define LOAX_BindFramebuffer                     6
#define LOAX_BindRenderbuffer                    7
#define LOAX_BindTexture                         8
#define LOAX_BlendColor                          9
#define LOAX_BlendEquation                       10
#define LOAX_BlendEquationSeparate               11
#define LOAX_BlendFunc                           12
#define LOAX_BlendFuncSeparate                   13
#define LOAX_BufferData                          14
#define LOAX_BufferSubData                       15
#define LOAX_CheckFramebufferStatus              16
#define LOAX_Clear                               17
#define LOAX_ClearColor                          18
#define LOAX_ClearDepthf                         19
#define LOAX_ClearStencil                        20
#define LOAX_ColorMask                           21
#define LOAX_CompileShader                       22
#define LOAX_CompressedTexImage2D                23
#define LOAX_CompressedTexSubImage2D             24
#define LOAX_CopyTexImage2D                      25
#define LOAX_CopyTexSubImage2D                   26
#define LOAX_CreateProgram                       27
#define LOAX_CreateShader                        28
#define LOAX_CullFace                            29
#define LOAX_DeleteBuffers                       30
#define LOAX_DeleteFramebuffers                  31
#define LOAX_DeleteProgram                       32
#define LOAX_DeleteRenderbuffers                 33
#define LOAX_DeleteShader                        34
#define LOAX_DeleteTextures                      35
#define LOAX_DepthFunc                           36
#define LOAX_DepthMask                           37
#define LOAX_DepthRangef                         38
#define LOAX_DetachShader                        39
#define LOAX_Disable                             40
#define LOAX_DisableVertexAttribArray            41
#define LOAX_DrawArrays                          42
#define LOAX_DrawElements                        43
#define LOAX_Enable                              44
#define LOAX_EnableVertexAttribArray             45
#define LOAX_Finish                              46
#define LOAX_Flush                               47
#define LOAX_FramebufferRenderbuffer             48
#define LOAX_FramebufferTexture2D                49
#define LOAX_FrontFace                           50
#define LOAX_GenBuffers                          51
#define LOAX_GenerateMipmap                      52
#define LOAX_GenFramebuffers                     53
#define LOAX_GenRenderbuffers                    54
#define LOAX_GenTextures                         55
#define LOAX_GetActiveAttrib                     56
#define LOAX_GetActiveUniform                    57
#define LOAX_GetAttachedShaders                  58
#define LOAX_GetAttribLocation                   59
#define LOAX_GetBooleanv                         60
#define LOAX_GetBufferParameteriv                61
#define LOAX_GetError                            62
#define LOAX_GetFloatv                           63
#define LOAX_GetFramebufferAttachmentParameteriv 64
#define LOAX_GetIntegerv                         65
#define LOAX_GetProgramiv                        66
#define LOAX_GetProgramInfoLog                   67
#define LOAX_GetRenderbufferParameteriv          68
#define LOAX_GetShaderiv                         69
#define LOAX_GetShaderInfoLog                    70
#define LOAX_GetShaderPrecisionFormat            71
#define LOAX_GetShaderSource                     72
#define LOAX_GetString                           73
#define LOAX_GetTexParameterfv                   74
#define LOAX_GetTexParameteriv                   75
#define LOAX_GetUniformfv                        76
#define LOAX_GetUniformiv                        77
#define LOAX_GetUniformLocation                  78
#define LOAX_GetVertexAttribfv                   79
#define LOAX_GetVertexAttribiv                   80
#define LOAX_GetVertexAttribPointerv             81
#define LOAX_Hint                                82
#define LOAX_IsBuffer                            83
#define LOAX_IsEnabled                           84
#define LOAX_IsFramebuffer                       85
#define LOAX_IsProgram                           86
#define LOAX_IsRenderbuffer                      87
#define LOAX_IsShader                            88
#define LOAX_IsTexture                           89
#define LOAX_LineWidth                           90
#define LOAX_LinkProgram                         91
#define LOAX_PixelStorei                         92
#define LOAX_PolygonOffset                       93
#define LOAX_ReadPixels                          94
#define LOAX_ReleaseShaderCompiler               95
#define LOAX_RenderbufferStorage                 96
#define LOAX_SampleCoverage                      97
#define LOAX_Scissor                             98
#define LOAX_ShaderBinary                        99
#define LOAX_ShaderSource                        100
#define LOAX_StencilFunc                         101
#define LOAX_StencilFuncSeparate                 102
#define LOAX_StencilMask                         103
#define LOAX_StencilMaskSeparate                 104
#define LOAX_StencilOp                           105
#define LOAX_StencilOpSeparate                   106
#define LOAX_TexImage2D                          107
#define LOAX_TexParameterf                       108
#define LOAX_TexParameterfv                      109
#define LOAX_TexParameteri                       110
#define LOAX_TexParameteriv                      111
#define LOAX_TexSubImage2D                       112
#define LOAX_Uniform1f                           113
#define LOAX_Uniform1fv                          114
#define LOAX_Uniform1i                           115
#define LOAX_Uniform1iv                          116
#define LOAX_Uniform2f                           117
#define LOAX_Uniform2fv                          118
#define LOAX_Uniform2i                           119
#define LOAX_Uniform2iv                          120
#define LOAX_Uniform3f                           121
#define LOAX_Uniform3fv                          122
#define LOAX_Uniform3i                           123
#define LOAX_Uniform3iv                          124
#define LOAX_Uniform4f                           125
#define LOAX_Uniform4fv                          126
#define LOAX_Uniform4i                           127
#define LOAX_Uniform4iv                          128
#define LOAX_UniformMatrix2fv                    129
#define LOAX_UniformMatrix3fv                    130
#define LOAX_UniformMatrix4fv                    131
#define LOAX_UseProgram                          132
#define LOAX_ValidateProgram                     133
#define LOAX_VertexAttrib1f                      134
#define LOAX_VertexAttrib1fv                     135
#define LOAX_VertexAttrib2f                      136
#define LOAX_VertexAttrib2fv                     137
#define LOAX_VertexAttrib3f                      138
#define LOAX_VertexAttrib3fv                     139
#define LOAX_VertexAttrib4f                      140
#define LOAX_VertexAttrib4fv                     141
#define LOAX_VertexAttribPointer                 142
#define LOAX_Viewport                            143
#define LOAX_MAX                                 144

#endif

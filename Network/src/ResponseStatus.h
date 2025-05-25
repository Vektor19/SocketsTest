#pragma once
namespace networking {
	enum ResponseStatus
	{
		OK = 200,
		NotFound = 404,
		MethodNotAllowed = 405,
		BadRequest = 400,
		Created = 201
	};
}